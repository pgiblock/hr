#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define ENV_SYMBOL "HR_SYMBOL"
#define DEFAULT_SYMBOL "="

static void
print_usage (FILE *f, char *exe)
{
  //TODO: New usage: hr [-s<line>] text to center
  fprintf(f, "Usage: %s [symbol]\n", exe);
}

int
main (int argc, char **argv)
{
  struct winsize ws;
  int len, i;

  char *sym, *tmp;
  switch (argc) {
    case 1:
      // No arguments: use symbol from env-var, if available
      tmp = getenv(ENV_SYMBOL);
      if (tmp) {
        sym = tmp;
        len = strlen(sym);
      } else {
        // Default symbol
        sym = DEFAULT_SYMBOL;
        len = 1;
      }
      break;

    case 2:
      // Argument specified: either a flag or a symbol
      if (strcmp(argv[1], "--help") == 0) {
        print_usage(stdout, argv[0]);
        printf("Displays a horizontal rule for finding your place in terminal history.\n");
        exit(0);
      }
      sym = argv[1];
      len = strlen(sym);
      break;

    default:
      // TODO: Concatenate all arguments with spaces (and print an extra space between repetitions)
      fprintf(stderr, "Error: unexpected number of arguments\n");
      print_usage(stderr, argv[0]);
      exit(1);
  }

  // Fetch the window size of the terminal
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

  // Up one line
  fputs("\033[A", stdout);

  // Print horizontal rule
  for (i = 0; i <= ws.ws_col-len; i += len) {
    fputs(sym, stdout);
  }
  // The remainder
  fwrite(sym, 1, ws.ws_col-i, stdout);

  return 0;
}

