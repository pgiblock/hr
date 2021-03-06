/*
 * Copyright 2013 Paul Giblock
 * This file is part of hr.
 * 
 * hr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * hr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with hr.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <iconv.h>
#include <langinfo.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <sys/ioctl.h>

#define ENV_SYMBOL "HR_SYMBOL"
#define DEFAULT_ASCII_SYMBOL "="
#define DEFAULT_UNICODE_SYMBOL "\u2550"

const char *program_name;

static void
print_usage (FILE *f)
{
  //TODO: New usage: hr [-s<line>] text to center
  fprintf(f, "Usage: %s [SYMBOL]\n", program_name);
  fputs("\
Display a horizontal rule using the string, SYMBOL.\n\n\
      --help     display this help and exit\n\
      --version  output version information and exit\n", f);
}


static void
print_version (FILE *f)
{
  fputs("\
hr 0.1\n\
Copyright (C) 2013 Paul Giblock\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n\n\
Written by Paul R. Giblock.\n", f);
}


static void
error (const char *format, int print_errmsg, ...)
{
  va_list args;
  // Preamble
  fputs(program_name, stderr);
  fputs(": ", stderr);
  // Program message
  va_start(args, print_errmsg);
  vfprintf(stderr, format, args);
  va_end(args);
  // Optional cause
  if (print_errmsg) {
    fputs(": ", stderr);
    fputs(strerror(errno), stderr);
  }
  fputc('\n', stderr);
}


int
main (int argc, char *argv[])
{
  struct winsize ws;
  int i, x;

  char    *sym, *wrptr;
  wchar_t *w_sym;
  size_t   in_cnt, out_cnt, len;

  program_name = argv[0];

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
        print_usage(stdout);
        exit(EXIT_SUCCESS);
      }
      if (strcmp(argv[1], "--version") == 0) {
        print_version(stdout);
        exit(EXIT_SUCCESS);
      }
      sym = argv[1];
      break;

    default:
      // TODO: Concatenate all arguments with spaces (and print an extra space between repetitions)
      error("unexpected number of arguments", 0);
      print_usage(stderr);
      exit(EXIT_FAILURE);
  }

  // Prepare for encoding conversion
  char *codeset = nl_langinfo(CODESET);
  iconv_t cd = iconv_open ("WCHAR_T", codeset);
  if (cd == (iconv_t) -1) {
    /* Something went wrong.  */
    if (errno == EINVAL) {
      error("conversion from `%s' to wchar_t not available", 1, codeset);
    } else {
      error("could not open character set conversion", 1);
    }
    exit(EXIT_FAILURE);
  }

  // Convert to wide chars for truncation
  in_cnt  = strlen(sym) + 1;
  out_cnt = in_cnt * sizeof(wchar_t);
  w_sym = malloc(out_cnt);
  wrptr = (char *) w_sym;

  if (iconv(cd, &sym, &in_cnt, &wrptr, &out_cnt) == (size_t)-1) {
    error("could not perform character set conversion", 1);
    exit(EXIT_FAILURE);
  }
  len = wcslen(w_sym);

  // Don't accept input that could never span the screen
  if (wcswidth(w_sym, len) <= 0) {
    error("symbol has invalid length", 0);
    exit(EXIT_FAILURE);
  }

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
    error("could not close character set conversion", 1);
    // Fall-thru: Whatever... just complete termination at this point
  }

  free(w_sym);
  return EXIT_SUCCESS;
}

