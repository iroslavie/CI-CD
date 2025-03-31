#ifndef S21_GREP_H

#define S21_GREP_H

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} Flags;

void which_flag(int argc, char *argv[], Flags *flags, int *samples_count,
                char **samples, int *sample_files_count, char **sample_files);

void work_with_samples(int *samples_count, char **samples,
                       int *samples_files_count, char **samples_files, int argc,
                       char **argv);

void work_with_files(int argc, char *argv[], char **files, int *count_files,
                     char **samples, int samples_count, int samples_files_count,
                     char **samples_files);

void file_processing(FILE *file, Flags *flags, int samples_count,
                     char **samples, int samples_files_count,
                     char **samples_files, char *file_name, int files_count,
                     int *match_count);

char *search_term_in_file(char *start_line, Flags *flags, int samples_count,
                          char **samples, int samples_files_count,
                          char **sample_files, int line_number, char *file_name,
                          int files_count, int *match_count);

char *sample_search(char *start_line, char *term, Flags flags,
                    regmatch_t *base);

char *out_with_0(char *line, Flags *flags, int line_nmb, char *file_name,
                 int files_count, int *switch_n, char *result, int flag,
                 char *sample_position);

void out_line(char *start_line, char *file_name, int line_nmb, int files_count,
              Flags flags);

#endif