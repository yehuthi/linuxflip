#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <libinput.h>
#include <libudev.h>
#include <fcntl.h>
#include <poll.h>

static int open_restricted(const char *path, int flags, void *user_data) {
	int fd = open(path, flags);
	return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data) {
	close(fd);
}

int main() {
	if (geteuid()) {
		fprintf(stderr, "Error: this utility requires root privileges (did you forget `sudo`?)\n");
		return 1;
	}

	const static struct libinput_interface file_open_close = {
		.open_restricted = open_restricted,
		.close_restricted = close_restricted,
	};

	struct udev *udev = udev_new();
	struct libinput *li = libinput_udev_create_context(&file_open_close, 0, udev);

	libinput_udev_assign_seat(li, "seat0");

	struct libinput_event *event;

	// Exhaust open device events
	libinput_dispatch(li);
	while ((event = libinput_get_event(li))) libinput_event_destroy(event);

	struct pollfd pollfd = {
		.fd = libinput_get_fd(li),
		.events = POLLIN,
		.revents = 0
	};

	while(poll(&pollfd, 1, -1)) {
		libinput_dispatch(li);
		while ((event = libinput_get_event(li))) {
			if (libinput_event_get_type(event) != LIBINPUT_EVENT_SWITCH_TOGGLE) continue;
			struct libinput_event_switch *switch_event = libinput_event_get_switch_event(event);
			if (libinput_event_switch_get_switch(switch_event) != LIBINPUT_SWITCH_TABLET_MODE) continue;
			const char switch_on = libinput_event_switch_get_switch_state(switch_event) == LIBINPUT_SWITCH_STATE_ON;
			printf("%s\n", switch_on ? "TABLET" : "LAPTOP");
			libinput_event_destroy(event);
		}
	}

	libinput_unref(li);
	udev_unref(udev);
	return 0;
}
