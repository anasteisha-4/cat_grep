#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE

int main(int argc, char **argv) {
  arguments flags = {0};
  flags_parser(argc, argv, &flags);
  do_grep(argc, argv, &flags);
  return 0;
}

void flags_parser(int argc, char **argv, arguments *flags) {
  int opt = getopt(argc, argv, "e:ivclnhsf:o");
  while (opt != -1) {
    switch (opt) {
      case 'e':
        flags->e = 1;
        add_pattern(flags, optarg);
        break;
      case 'i':
        flags->i = REG_ICASE;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        add_reg_from_file(flags, optarg);
        break;
      case 'o':
        flags->o = 1;
        break;
    }
    opt = getopt(argc, argv, "e:ivclnhsf:o");
  }
  if (flags->len_p == 0) {
    add_pattern(flags, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) flags->h = 1;
}

void do_grep(int argc, char **argv, arguments *flags) {
  regex_t re;
  int err = regcomp(&re, flags->pattern, REG_EXTENDED | flags->i);
  if (err)
    printf("s21_grep: RegError\n");
  else {
    for (int i = optind; i < argc; i++) {
      process_file(flags, argv[i], &re);
    }
    regfree(&re);
  }
}

void process_file(arguments *flags, char *filename, regex_t *re) {
  FILE *file = fopen(filename, "r");

  if (!file) {
    if (!flags->s) printf("s21_grep: %s: No such file\n", filename);
  } else {
    char *line = NULL;
    size_t buf = 0;
    long n = 0;
    int line_idx = 1, match_line_cnt = 0;

    while ((n = (long)getline(&line, &buf, file)) != -1) {
      int res = regexec(re, line, 0, NULL, 0);

      if ((!res && !flags->v) || (flags->v && res)) {
        if (!flags->c && !flags->l && !flags->h && !flags->o)
          printf("%s:", filename);
        if (!flags->c && !flags->l && flags->n && !flags->o)
          printf("%d:", line_idx);
        if (!flags->c && !flags->l && flags->o)
          print_match(re, line, flags, filename, line_idx);
        if (!flags->c && !flags->l && !flags->o) print_line(line, strlen(line));

        match_line_cnt++;
      }

      line_idx++;
    }
    if (line) free(line);

    if (flags->c && !flags->l) {
      if (!flags->h) printf("%s:", filename);
      printf("%d\n", match_line_cnt);
    }

    if (flags->l && match_line_cnt > 0) printf("%s\n", filename);
    fclose(file);
  }
}

void add_reg_from_file(arguments *flags, char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    if (!flags->s) printf("s21_grep: %s: No such file\n", filename);
  } else {
    char *line = NULL;
    size_t buf = 0;
    long n = 0;
    while ((n = (long)getline(&line, &buf, file)) != -1) {
      if (line[n - 1] == '\n') line[n - 1] = '\0';
      add_pattern(flags, line);
    }
    if (line) free(line);
    fclose(file);
  }
}

void add_pattern(arguments *flags, char *pattern) {
  if (flags->len_p != 0) {
    strcat(flags->pattern + flags->len_p, "|");
    (flags->len_p)++;
  }
  (flags->len_p) += sprintf(flags->pattern + flags->len_p, "(%s)", pattern);
}

void print_match(regex_t *re, char *line, arguments *flags, char *filename,
                 int line_idx) {
  regmatch_t match;
  int res = regexec(re, line, 1, &match, 0);
  int offset = 0;
  while (res == 0) {
    if (!flags->h) printf("%s:", filename);
    if (flags->n) printf("%d:", line_idx);
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[offset + i]);
    }
    putchar('\n');
    offset += match.rm_eo;
    res = regexec(re, line + offset, 1, &match, 0);
  }
}

void print_line(char *line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') putchar('\n');
}
