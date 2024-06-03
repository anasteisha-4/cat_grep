#ifndef S21_CAT_H
#define S21_CAT_H

typedef struct arguments {
  int b, n, s, v, E, T;
} arguments;

arguments flags_parser(int argc, char **argv, int *err);

void print_files(int argc, char **argv, arguments flags);
int print_file(char *filename, arguments flags, int *line_idx, char *prev);
void print_char(char c, char *prev, arguments flags, int *line_idx,
                int *blank_line);

#endif