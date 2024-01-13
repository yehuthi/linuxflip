#include "linuxflip.h"
#include <errno.h>
#include <fcntl.h>
#include <libinput.h>
#include <libudev.h>
#include <unistd.h>
#include <poll.h>

static int open_restricted(const char *path, int flags, void *user_data) {
	int fd = open(path, flags);
	return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data) { close(fd); }

enum linuxflip_result linuxflip_init(struct linuxflip *restrict linuxflip) {
	static const struct libinput_interface file_open_close = {
		.open_restricted  = open_restricted,
		.close_restricted = close_restricted,
	};
	linuxflip->udev = udev_new();
	if (!linuxflip->udev) return LINUXFLIP_ERROR_UDEV_INIT;
	linuxflip->libinput = libinput_udev_create_context(&file_open_close, 0, linuxflip->udev);
	if (!linuxflip->libinput) {
		udev_unref(linuxflip->udev);
		return LINUXFLIP_ERROR_LIBINPUT_INIT;
	}
	if (libinput_udev_assign_seat(linuxflip->libinput, "seat0") == -1) {
		libinput_unref(linuxflip->libinput);
		udev_unref(linuxflip->udev);
		return LINUXFLIP_ERROR_LIBINPUT_ASSIGN_SEAT;
	}

	return LINUXFLIP_OK;
}

void linuxflip_go(struct linuxflip linuxflip, linuxflip_hook hook, void *data) {
	struct pollfd pollfd = {
		.fd = libinput_get_fd(linuxflip.libinput),
		.events = POLLIN,
		.revents = 0
	};

	struct libinput_event *event;

	// Exhaust open device events
	libinput_dispatch(linuxflip.libinput);
	while ((event = libinput_get_event(linuxflip.libinput))) libinput_event_destroy(event);

	do {
		libinput_dispatch(linuxflip.libinput);
		while ((event = libinput_get_event(linuxflip.libinput))) {
			if (libinput_event_get_type(event) != LIBINPUT_EVENT_SWITCH_TOGGLE) continue;
			struct libinput_event_switch *switch_event = libinput_event_get_switch_event(event);
			if (libinput_event_switch_get_switch(switch_event) != LIBINPUT_SWITCH_TABLET_MODE) continue;
			const linuxflip_state state = libinput_event_switch_get_switch_state(switch_event);
			bool should_continue = hook(state, data);
			libinput_event_destroy(event);
			if (!should_continue) goto out;
		}
	} while(poll(&pollfd, 1, -1));
out:;
}

void linuxflip_free(struct linuxflip linuxflip) {
	libinput_unref(linuxflip.libinput);
	udev_unref(linuxflip.udev);
}
