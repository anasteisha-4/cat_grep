#ifndef S21_GREP_H
#define S21_GREP_H

#include <regex.h>

typedef struct arguments {
  int e, i, v, c, l, n, h, s, f, o;
  size_t len_p;
  char pattern[50000];
} arguments;

void flags_parser(int argc, char **argv, arguments *flags);
void do_grep(int argc, char **argv, arguments *flags);
void process_file(arguments *flags, char *filename, regex_t *re);
void add_reg_from_file(arguments *flags, char *filename);
void add_pattern(arguments *flags, char *pattern);
void print_match(regex_t *re, char *line, arguments *flags, char *filename,
                 int line_idx);
void print_line(char *line, int n);

#endif