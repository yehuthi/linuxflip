#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "linuxflip.h"

#define FATAL(...) {                                                                  \
	fprintf(stderr, isatty(STDERR_FILENO) ? "\x1b[;31mError:\x1b[;39m " : "Error: "); \
	fprintf(stderr, __VA_ARGS__);                                                     \
	exit(1);                                                                          \
}

#define STRINGIFY(a) #a
#define TO_STRING(a) STRINGIFY(a)

static int print_faded(const char* string) { return printf("\x1b[;90m%s\x1b[;39m", string); }
static int print_highlight(const char* string) { return printf("\x1b[;33m%s\x1b[;39m", string); }
static int print(const char* string) { return fputs(string, stdout); }
void print_help(const char *exe, bool color) {
	struct {
		const char *args;
		const char *description;
	} clauses[] = {
		{.args = "", .description = "Prints the mode (\"tablet\" or \"laptop\") to stdout when it changes."},
		{.args = "<toggle-cmd>", .description = "Runs <toggle-cmd> in a shell whenever the mode changes."},
		{.args = "<tablet-cmd> <laptop-cmd>", .description = "Runs <tablet-cmd> in a shell when the mode changes to tablet, and <laptop-cmd> when the mode changes to laptop."},
		{.args = "--help", .description = "Prints out this help"},
#ifdef LINUXFLIP_VERSION
		{.args = "--version", .description = "Prints version"},
#endif
	};

	int (*print_exe)(const char* string) = color ? print_faded : print;
	int (*print_args)(const char* string) = color ? print_highlight : print;

	puts(color ? "\x1b[;1mUsage:\x1b[;22m" : "Usage:");
	for (size_t i = 0; i < sizeof(clauses) / sizeof(clauses[0]); i++) {
		printf("  ");
		print_exe(exe);
		putc(' ', stdout);
		print_args(clauses[i].args);
		printf("\n      %s\n", clauses[i].description);
	}
}

static bool hook(linuxflip_state state, void *data) {
	char* command = ((char**)data)[state];
	if (fork() == 0)
		execl("/bin/sh", "sh", "-c", command, (char*)0);
	return true;
}

const char* linuxflip_result_display[] = {
	"",
	"failed to initialize a udev context",
	"failed to initialize a libinput context",
	"failed to assign a seat",
};

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

	enum linuxflip_result result;

	struct linuxflip lf;
	result = linuxflip_init(&lf);
	if (result != LINUXFLIP_OK) {
		FATAL("%s\n", linuxflip_result_display[result]);
	}
	// TODO: check result
	char* data[] = {
		cmd_laptop,
		cmd_tablet,
	};
	linuxflip_go(lf, hook, data);
	// linuxflip_free(lf); // mentioned but commented out because unreachable + OS would clean it up better anyway
	return 0;
}
