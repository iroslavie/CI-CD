#ifndef S21_CAT_H

#define S21_CAT_H

#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool no_empty_strings;
  bool dollar_at_the_end;
  bool numbering_all;
  bool compression;
  bool tabs;
  bool non_printed;
} Flags;

Flags which_flag(int argc, char *argv[]);

void ascii_table(const char *table[static 256]);

void cat_dollar_at_the_end(const char *table[static 256]);

void cat_tabs(const char *table[static 256]);

void cat_non_printed(const char *table[static 256]);

void cat_print_file(FILE *file, Flags flags, const char *table[static 256]);

void cat_open_file_or_stdin(int argc, char *argv[], Flags flags,
                            const char *table[static 256]);

#endif