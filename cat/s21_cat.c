#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  int err = 0;
  arguments flags = flags_parser(argc, argv, &err);
  if (!err) print_files(argc, argv, flags);
  return 0;
}

arguments flags_parser(int argc, char **argv, int *err) {
  arguments flags = {0};
  struct option long_flags[] = {{"number", no_argument, NULL, 'n'},
                                {"number-nonblank", no_argument, NULL, 'b'},
                                {"squeeze-blank", no_argument, NULL, 's'},
                                {0, 0, 0, 0}};

  int opt = getopt_long(argc, argv, "bnsvETet", long_flags, 0);
  while (opt != -1) {
    switch (opt) {
      case 'b':
        flags.b = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
      case 's':
        flags.s = 1;
        break;
      case 'v':
        flags.v = 1;
        break;
      case 'E':
        flags.E = 1;
        break;
      case 'T':
        flags.T = 1;
        break;
      case 'e':
        flags.E = 1;
        flags.v = 1;
        break;
      case 't':
        flags.T = 1;
        flags.v = 1;
        break;
      default:
        *err = 1;
        break;
    }
    opt = getopt_long(argc, argv, "bnsvETet", long_flags, 0);
  }
  return flags;
}

void print_files(int argc, char **argv, arguments flags) {
  int line_idx = 0;
  char prev = '\n';
  if (optind < argc) {
    for (int i = optind; i < argc; i++) {
      if (print_file(argv[i], flags, &line_idx, &prev))
        printf("s21_cat: %s: No such file\n", argv[i]);
    }
  } else {
    print_file("-", flags, &line_idx, &prev);
  }
}

int print_file(char *filename, arguments flags, int *line_idx, char *prev) {
  FILE *file = NULL;
  int err = 0;

  if (!strcmp(filename, "-")) {
    file = stdin;
  } else {
    file = fopen(filename, "r");
  }

  if (file) {
    int blank_line = 0;
    char c = fgetc(file);
    while (!feof(file)) {
      print_char(c, prev, flags, line_idx, &blank_line);
      c = fgetc(file);
    }
    if (file != stdin) {
      fclose(file);
    }
  } else {
    err = 1;
  }
  return err;
}

void print_char(char c, char *prev, arguments flags, int *line_idx,
                int *blank_line) {
  if (!flags.s || *prev != '\n' || c != '\n' || *blank_line == 0) {
    *blank_line = (*prev == '\n' && c == '\n') ? 1 : 0;

    if (((flags.n && !flags.b) || (flags.b && c != '\n')) && *prev == '\n') {
      (*line_idx)++;
      printf("%6d\t", *line_idx);
    }

    if (flags.E && c == '\n') {
      putchar('$');
    }

    if (flags.T && c == '\t') {
      putchar('^');
      c += 64;
    }

    if (flags.v) {
      if (c < 0) {
        printf("M-");
        c &= 0x7F;
      }

      if (0 <= c && c <= 31 && c != '\n' && c != '\t') {
        putchar('^');
        c += 64;
      } else if (c == 127) {
        putchar('^');
        c = '?';
      }
    }

    putchar(c);
  }

  *prev = c;
}