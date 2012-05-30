#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define HR_SYMBOL "HR_SYMBOL"

static void
print_usage (FILE *f, char *exe) {
	//TODO: New usage: hr [-s<line>] text to center
	fprintf(f, "Usage: %s [symbol]\n", exe);
}

int
main (int argc, char **argv) {
	struct winsize ws;
	int len, i;

	char *str, *tmp;
	switch (argc) {
		case 1:
			tmp = getenv(HR_SYMBOL);
			if (tmp) {
				str = tmp;
				len = strlen(tmp);
			} else {
				str = "=";
				len = 1;
			}
			break;

		case 2:
			if (!strcmp(argv[1], "--help")) {
				print_usage(stdout, argv[0]);
				printf("Displays a horizontal rule for finding your place in terminal history.\n");
				exit(0);
			}
			str = argv[1];
			len = strlen(str);
			break;

		default:
			// TODO: Concatenate all arguments with spaces (and print an extra space between repetitions)
			fprintf(stderr, "Error: unexpected number of arguments\n");
			print_usage(stderr, argv[0]);
			exit(1);
	}
			
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

	// Up one line
	fputs("\033[A", stdout);

	// Print horizontal rule
	// TODO: Handle cols < len
	for (i = 0; i <= ws.ws_col-len; i += len) {
		fputs(str, stdout);
	}
	// The remainder
	fwrite(str, 1, ws.ws_col-i, stdout);

	return 0;
}

