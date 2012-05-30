#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

int
main (int argc, char **argv) {
	struct winsize ws;
	int i;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

	// Up one line
	fputs("\033[A", stdout);

	// Print horizontal rule
	for (i=0; i<ws.ws_col; ++i) {
		putchar('=');
	}
	return 0;
}

