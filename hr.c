#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iconv.h>
#include <langinfo.h>
#include <locale.h>
#include <errno.h>
#include <error.h>
#include <wchar.h>

#define ENV_SYMBOL "HR_SYMBOL"
#define DEFAULT_ASCII_SYMBOL "="
#define DEFAULT_UNICODE_SYMBOL "\u2550"

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
  int i, x;

  char    *sym, *wrptr;
  wchar_t *w_sym;
  size_t   in_cnt, out_cnt, len;

  setlocale(LC_ALL, "");

  switch (argc) {
    case 1:
      // No arguments: use symbol from env-var, if available
      sym = getenv(ENV_SYMBOL);
      if (!sym) {
        // Default symbol
        if (MB_CUR_MAX == 1) {
          sym = DEFAULT_ASCII_SYMBOL;
        } else {
          sym = DEFAULT_UNICODE_SYMBOL;
        }
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
      break;

    default:
      // TODO: Concatenate all arguments with spaces (and print an extra space between repetitions)
      fprintf(stderr, "Error: unexpected number of arguments\n");
      print_usage(stderr, argv[0]);
      exit(1);
  }

  // Prepare for encoding conversion
  char *codeset = nl_langinfo(CODESET);
  iconv_t cd = iconv_open ("WCHAR_T", codeset);
  if (cd == (iconv_t) -1) {
    /* Something went wrong.  */
    if (errno == EINVAL) {
      error (0, 0, "conversion from '%s' to wchar_t not available",
          codeset);
    } else {
      perror ("iconv_open");
    }
    exit(1);
  }

  // Convert to wide chars for truncation
  in_cnt  = strlen(sym) + 1;
  out_cnt = in_cnt * sizeof(wchar_t);
  w_sym = malloc(out_cnt);
  wrptr = (char *) w_sym;
  // iconv(cd, NULL, NULL, &wrptr, &out_cnt);
  if (iconv(cd, &sym, &in_cnt, &wrptr, &out_cnt) == (size_t)-1) {
    // ERROR
  } 
  len = wcslen(w_sym);

  // Fetch the window size of the terminal
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

  // Up one line
  fputws(L"\033[A", stdout);
  fflush(stdout);

  // Print horizontal rule
  for (x = i = 0; x < ws.ws_col; x += wcwidth(w_sym[i])) {
    fputwc(w_sym[i], stdout);
    i = (i+1) % len;
  }


  if (iconv_close(cd) != 0) {
    perror("iconv_close");
  }

  free(w_sym);

  return 0;
}

