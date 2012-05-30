#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

static void
print_usage (char *exe) {
	fprintf(stderr, "Usage: %s [symbol]\n", exe);
}

int
main (int argc, char **argv) {
	struct winsize ws;
	int len, i;

	char *str;
	switch (argc) {
		case 1:
			str = "=";
			len = 1;
			break;
		case 2:
			str = argv[1];
			len = strlen(str);
			break;
		default:
			// TODO: Concatenate all arguments with spaces (and print an extra space between repetitions)
			print_usage(argv[0]);
			exit(1);
	}
			
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

	// Up one line
	fputs("\033[A", stdout);

	// Print horizontal rule
	for (i = 0; i <= ws.ws_col-len; i += len) {
		fputs(str, stdout);
	}
	// TODO: Or just newline if i < cols?
	for (; i < ws.ws_col; ++i) {
		putchar(' ');
	}
	return 0;
}

