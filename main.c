#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <libinput.h>
#include <libudev.h>
#include <fcntl.h>
#include <poll.h>
#include <stdbool.h>
#include "cli.h"

#define FATAL(...) {                                                                  \
	fprintf(stderr, isatty(STDERR_FILENO) ? "\x1b[;31mError:\x1b[;39m " : "Error: "); \
	fprintf(stderr, __VA_ARGS__);                                                     \
	exit(1);                                                                          \
}

#define STRINGIFY(a) #a
#define TO_STRING(a) STRINGIFY(a)

static int open_restricted(const char *path, int flags, void *user_data) {
	int fd = open(path, flags);
	return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data) {
	close(fd);
}

int main(int argc, char *argv[]) {
	if (argc >= 2 && strcmp(argv[1],"--help") == 0) {
		print_help(argv[0], isatty(STDOUT_FILENO));
		return 0;
	}
#ifdef LINUXFLIP_VERSION
	if (argc >= 2 && strcmp(argv[1],"--version") == 0) {
		printf("linuxflip %s\n", TO_STRING(LINUXFLIP_VERSION));
		return 0;
	}
#endif

	if (argc > 3)
		FATAL("excess arguments (%d, max of 2) were supplied (did you forget to quote the arguments?)\n", argc - 1);

	char* cmd_tablet = "echo tablet";
	char* cmd_laptop = "echo laptop";
	if (argc >= 2) cmd_tablet = cmd_laptop = argv[1];
	if (argc >= 3) cmd_laptop = argv[2];

	if (geteuid()) FATAL("this utility requires root privileges (did you forget `sudo`?)\n");

	const struct libinput_interface file_open_close = {
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

	// Start the event loop
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
			const bool tablet_mode = libinput_event_switch_get_switch_state(switch_event) == LIBINPUT_SWITCH_STATE_ON;
			if (!fork())
				exit(system(tablet_mode ? cmd_tablet : cmd_laptop));
			libinput_event_destroy(event);
		}
	}

	libinput_unref(li);
	udev_unref(udev);
	return 0;
}
