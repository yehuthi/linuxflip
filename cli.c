#include "cli.h"
#include <stdio.h>

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
	for (int i = 0; i < sizeof(clauses) / sizeof(clauses[0]); i++) {
		printf("  ");
		print_exe(exe);
		putc(' ', stdout);
		print_args(clauses[i].args);
		printf("\n      %s\n", clauses[i].description);
	}
}
