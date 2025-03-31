#include "s21_grep.h"

int main(int argc, char *argv[]) {
  // Инициализация
  Flags flags = {0};
  char *files[argc];
  char *samples[argc];
  char *samples_files[argc];
  int count_files = 0;
  int samples_count = 0;
  int samples_files_count = 0;
  int match_count = 0;

  // Обработка флагов и аргументов
  which_flag(argc, argv, &flags, &samples_count, samples, &samples_files_count,
             samples_files);

  // Обработка шаблонов
  work_with_samples(&samples_count, samples, &samples_files_count,
                    samples_files, argc, argv);

  // Обработка файлов
  work_with_files(argc, argv, files, &count_files, samples, samples_count,
                  samples_files_count, samples_files);

  // Проверка на наличие шаблонов
  if (samples_count == 0 && samples_files_count == 0) {
    fprintf(stderr, "%s: No patterns, man\n", argv[0]);
    exit(1);
  }
  FILE *file = NULL;
  // Обработка каждого файла
  for (int i = 0; i < count_files; i++) {
    file = fopen(files[i], "rb");
    if (file) {
      // Обработка содержимого файла
      file_processing(file, &flags, samples_count, samples, samples_files_count,
                      samples_files, files[i], count_files, &match_count);
    }
    fclose(file);
  }
}

void which_flag(int argc, char *argv[], Flags *flags, int *samples_count,
                char **samples, int *sample_files_count, char **sample_files) {
  int opt;
  int option_index;
  static struct option long_options[] = {{0, 0, 0, 0}};
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'f':
        flags->f = 1;
        sample_files[*sample_files_count] = optarg;
        *sample_files_count += 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'e':
        flags->e = 1;
        samples[*samples_count] = optarg;
        *samples_count += 1;
        break;
      case 'i':
        flags->i = 1;
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
      case 'o':
        flags->o = 1;
        break;
      default:
        fprintf(stderr, "%s: invalid option -- %c\n", argv[0], opt);
        exit(1);
    }
  }
}

void work_with_samples(int *samples_count, char **samples,
                       int *samples_files_count, char **samples_files, int argc,
                       char **argv) {
  // Если шаблоны не были заданы явно, извлечь из аргументов командной строки
  if (*samples_count == 0 && *samples_files_count == 0) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] != '-' && argv[i][0] != 0) {
        samples[*samples_count] = argv[i];
        *samples_count += 1;
        break;
      }
    }
  }
  FILE *file = NULL;
  // Проверка существования файлов-шаблонов
  for (int i = 0; i < *samples_files_count; i++) {
    file = fopen(samples_files[i], "rb");
    if (file == NULL) {
      fprintf(stderr, "%s, No such file in directory\n", samples_files[i]);
      exit(1);
    }
    fclose(file);
  }
}

void work_with_files(int argc, char *argv[], char **files, int *count_files,
                     char **samples, int samples_count, int samples_files_count,
                     char **samples_files) {
  for (int i = 1; i < argc; i++) {
    int check = 0;
    if (argv[i][0] != '-') {
      // Проверка на явление аргумента шаблоном
      for (int j = 0; j < samples_count; j++) {
        if (argv[i] == samples[j]) {
          check += 1;
          break;
        }
      }
      // Проверка, не является ли аргумент именем файла шаблона
      for (int j = 0; j < samples_files_count; j++) {
        if (argv[i] == samples_files[j]) {
          check += 1;
          break;
        }
      }
      // Если аргумент не шаблон, добавляем его в список файлов для обработки
      if (check == 0) {
        files[*count_files] = argv[i];
        *count_files += 1;
      }
    }
  }
}

void file_processing(FILE *file, Flags *flags, int samples_count,
                     char **samples, int samples_files_count,
                     char **samples_files, char *file_name, int files_count,
                     int *match_count) {
  size_t length;
  int line_number = 1;
  int end_line;
  char *line_from_file = NULL;
  // Чтение файла
  while ((end_line = getline(&line_from_file, &length, file)) != EOF) {
    int match_check = 0;
    if (line_from_file != NULL) {
      // Поиск шаблонов в строке файла
      char *mach_part = search_term_in_file(
          line_from_file, flags, samples_count, samples, samples_files_count,
          samples_files, line_number, file_name, files_count, match_count);
      if (mach_part == NULL) {
        match_check = 0;
      } else {
        match_check = 1;
      }
      // Вывод строки в зависимости от наличия совпадений и опций
      if ((match_check == 1 && flags->v == 0) ||
          (match_check == 0 && flags->v == 1)) {
        *match_count += 1;
        if (flags->c == 0 && flags->l == 0) {
          out_line(line_from_file, file_name, line_number, files_count, *flags);
        }
      }
    }
    line_number++;
  }
  free(line_from_file);
  if (flags->c) {
    if (flags->l) {
      *match_count = *match_count > 0 ? 1 : 0;
    }
    if (files_count == 1 || flags->h == 1) {
      fprintf(stdout, "%d\n", *match_count);
    } else {
      fprintf(stdout, "%s;%d\n", file_name, *match_count);
    }
  }
  if (flags->l == 1) {
    if (*match_count != 0) {
      fprintf(stdout, "%s\n", file_name);
    }
  }
}

char *search_term_in_file(char *start_line, Flags *flags, int samples_count,
                          char **samples, int samples_files_count,
                          char **sample_files, int line_number, char *file_name,
                          int files_count, int *match_count) {
  char *result = NULL;
  char *result_1 = NULL;
  int check = 0;
  size_t length;
  regmatch_t base[strlen(start_line)];
  char *sample_from_file = NULL;
  int switch_n = 0;
  char *sample_position = NULL;
  int mass[2] = {-1, -1};
  // Поиск шаблонов в строке
  for (int i = 0; i < samples_count; ++i) {
    result = sample_search(start_line, samples[i], *flags, base);
    if ((result != NULL && flags->o) || (result != NULL && flags->v)) {
      break;
    } else if (result != NULL && flags->o) {
      mass[0] = mass[i];
      mass[i] = i;
      if (sample_position == NULL) *match_count += 1;
      char sample[strlen(start_line)];
      int len = base[0].rm_eo - base[0].rm_so;
      memcpy(sample, start_line + base[0].rm_so, len);
      sample[len] = 0;
      if ((sample_position != NULL && strstr(sample_position, sample)) ||
          sample_position == NULL) {
        int flagg = 0;
        sample_position =
            out_with_0(result, flags, line_number, file_name, files_count,
                       &switch_n, sample, flagg, sample_position);
      }
    }
    if (result != NULL) {
      check = -1;
      result_1 = result;
    }
  }
  // Если не найдено совпаденийб проверить файлы-шаблоны
  if (result == NULL) {
    for (int i = 0; i < samples_files_count; i++) {
      FILE *file = fopen(sample_files[i], "rb");
      int c;
      // Чтение файла-шаблона
      while ((c = getline(&sample_from_file, &length, file)) != EOF) {
        result = sample_search(start_line, sample_from_file, *flags, base);
        if ((result != NULL && !flags->o) || (result != NULL && flags->v)) {
          break;
        } else if (result != NULL && flags->o) {
          if (sample_position == NULL) {
            *match_count += 1;
          }
          char sample[strlen(start_line)];
          int len = base[0].rm_eo - base[0].rm_so;
          memcpy(sample, start_line + base[0].rm_so, len);
          sample[len] = 0;
          if ((sample_position != NULL && strstr(sample_position, sample)) ||
              sample_position == NULL) {
            int flagg = 0;
            sample_position =
                out_with_0(result, flags, line_number, file_name, files_count,
                           &switch_n, sample, flagg, sample_position);
          }
        }
      }
      free(sample_from_file);
      fclose(file);
      // Переключение флага -о, если не включен
      if (switch_n == 1 && !flags->o) {
        flags->o = true;
      }
    }
  }
  if (check != -1) {
    result_1 = result;
  }
  return result_1;
}

char *sample_search(char *start_line, char *term, Flags flags,
                    regmatch_t *base) {
  char *result = NULL;
  // Проверка, что строка не является пустой и не состоит только из символа
  // новой строки
  if (start_line[0] != 0 && (start_line[0] != '\n' || flags.f)) {
    // Структура для представлния регуляного выражения
    regex_t regex_term;
    int error = 0;
    int res = 0;
    // Буфер для ошибок
    char message[128];
    int lenght = strlen(start_line);
    // Компиляция регулярного выражения с учетом флага -i
    if (flags.i) {
      error = regcomp(&regex_term, term, REG_EXTENDED | REG_ICASE);
    } else {
      error = regcomp(&regex_term, term, REG_EXTENDED);
    }
    if (error != 0) {
      regerror(error, &regex_term, message, 128);
      printf("%s\n", message);
      exit(1);
    }
    // Поиск шаблона в строке
    if ((res = regexec(&regex_term, start_line, lenght, base, 0)) == 0) {
      result = start_line;
    } else if (res != REG_NOMATCH) {
      regerror(error, &regex_term, message, 128);
      printf("%s\n", message);
      exit(1);
    }
    regfree(&regex_term);
  }
  return result;
}

char *out_with_0(char *line, Flags *flags, int line_nmb, char *file_name,
                 int files_count, int *switch_n, char *result, int flagg,
                 char *sample_position) {
  char *new_line = NULL;
  // Текущая обрабатываемая строка, начинается с начала строки или шаблона
  char *current_line = NULL;
  // Определение начальной позиции для обработки строки
  if (sample_position != NULL) {
    current_line = sample_position;
  } else {
    current_line = line;
  }
  // Поиск и обработка всех вхождений шаблона в строке
  while ((sample_position = strstr(current_line, result)) != NULL) {
    // Замена первого вхождения шаблона точкой, если оно найдено
    if (strstr(sample_position, result) != NULL) {
      *sample_position = '.';
    }

    out_line(result, file_name, line_nmb, files_count, *flags);

    // Если флаг n не был активирован и программа встретила первое вхождение
    // шаблона
    if (*switch_n == 0 && flags->n == 1) {
      flags->n = 0;
      *switch_n = 1;
    }
    new_line = sample_position;
    if (flagg) {
      break;
    }
  }
  return new_line;
}

void out_line(char *start_line, char *file_name, int line_nmb, int files_count,
              Flags flags) {
  // Проверка наличия только одного файла или активации флага -h
  if (files_count == 1 || flags.h == 1) {
    // Если активирован флаг -n, выводится номер строки перед строкой
    if (flags.n) {
      printf("%d:%s", line_nmb, start_line);
    } else {
      printf("%s", start_line);
    }
  } else {
    // Вывод с информацией о файле и номере строки, если активирован флаг -n
    if (flags.n) {
      printf("%s:%d:%s", file_name, line_nmb, start_line);
    } else {
      printf("%s:%s", file_name, start_line);
    }
  }
  // Добавление символа новой строки, если строка не заканчивается символом
  // новой строки
  if (start_line[strlen(start_line) - 1] != '\n') {
    printf("\n");
  }
}