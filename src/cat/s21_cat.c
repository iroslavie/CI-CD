#include "s21_cat.h"

int main(int argc, char **argv) {
  Flags flags = which_flag(argc, argv);  // Получение флагов из клмандной строки
  const char *table[256];  // Инициализация ASCII таблицы
  ascii_table(table);
  if (flags.dollar_at_the_end)
    cat_dollar_at_the_end(table);  // Обработка флагов и работа с ними
  if (flags.tabs) cat_tabs(table);
  if (flags.non_printed) cat_non_printed(table);
  cat_open_file_or_stdin(argc, argv, flags,
                         table);  // Вывод из файлов или stdin с учетом флагов
  return 0;
}

Flags which_flag(int argc, char *argv[]) {
  // Поиск флагов с помощью getopt_long
  struct option longFlags[] = {{"number-nonblank", 0, NULL, 'b'},
                               {"number", 0, NULL, 'n'},
                               {"squeeze-blank", 0, NULL, 's'},
                               {NULL, 0, NULL, 0}};
  int that_flag = getopt_long(argc, argv, "bevEnstT", longFlags, NULL);

  Flags flags = {false, false, false, false, false, false};

  // обработка флагов
  for (; that_flag != -1;
       that_flag = getopt_long(argc, argv, "bevEnstT", longFlags, NULL)) {
    switch (that_flag) {
      case 'b':
        flags.no_empty_strings = true;
        break;
      case 'e':
        flags.dollar_at_the_end = true;
        flags.non_printed = true;
        break;
      case 'v':
        flags.non_printed = true;
        break;
      case 'E':
        flags.dollar_at_the_end = true;
        break;
      case 'n':
        flags.numbering_all = true;
        break;
      case 's':
        flags.compression = true;
        break;
      case 't':
        flags.non_printed = true;
        flags.tabs = true;
        break;
      case 'T':
        flags.tabs = true;
        break;
      default:
        printf("usage: %s [-benstuv] [file ...]", argv[0]);
        exit(1);
    }
  }
  return (flags);
}

void ascii_table(const char *table[static 256]) {
  const char *raw_table[] = {
      "\00",  "\01",  "\02",  "\03",  "\04",  "\05",  "\06",  "\07",  "\b",
      "\t",   "\n",   "\v",   "\014", "\r",   "\016", "\017", "\020", "\021",
      "\022", "\023", "\024", "\025", "\026", "\027", "\030", "\031", "\032",
      "\033", "\034", "\035", "\036", "\037", " ",    "!",    "\"",   "#",
      "$",    "%",    "&",    "\'",   "(",    ")",    "*",    "+",    ",",
      "-",    ".",    "/",    "0",    "1",    "2",    "3",    "4",    "5",
      "6",    "7",    "8",    "9",    ":",    ";",    "<",    "=",    ">",
      "?",    "@",    "A",    "B",    "C",    "D",    "E",    "F",    "G",
      "H",    "I",    "J",    "K",    "L",    "M",    "N",    "O",    "P",
      "Q",    "R",    "S",    "T",    "U",    "V",    "W",    "X",    "Y",
      "Z",    "[",    "\\",   "]",    "^",    "_",    "`",    "a",    "b",
      "c",    "d",    "e",    "f",    "g",    "h",    "i",    "j",    "k",
      "l",    "m",    "n",    "o",    "p",    "q",    "r",    "s",    "t",
      "u",    "v",    "w",    "x",    "y",    "z",    "{",    "|",    "}",
      "~",    "\x7f", "\x80", "\x81", "\x82", "\x83", "\x84", "\x85", "\x86",
      "\x87", "\x88", "\x89", "\x8a", "\x8b", "\x8c", "\x8d", "\x8e", "\x8f",
      "\x90", "\x91", "\x92", "\x93", "\x94", "\x95", "\x96", "\x97", "\x98",
      "\x99", "\x9a", "\x9b", "\x9c", "\x9d", "\x9e", "\x9f", "\xa0", "\xa1",
      "\xa2", "\xa3", "\xa4", "\xa5", "\xa6", "\xa7", "\xa8", "\xa9", "\xaa",
      "\xab", "\xac", "\xad", "\xae", "\xaf", "\xb0", "\xb1", "\xb2", "\xb3",
      "\xb4", "\xb5", "\xb6", "\xb7", "\xb8", "\xb9", "\xba", "\xbb", "\xbc",
      "\xbd", "\xbe", "\xbf", "\xc0", "\xc1", "\xc2", "\xc3", "\xc4", "\xc5",
      "\xc6", "\xc7", "\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcd", "\xce",
      "\xcf", "\xd0", "\xd1", "\xd2", "\xd3", "\xd4", "\xd5", "\xd6", "\xd7",
      "\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xdd", "\xde", "\xdf", "\xe0",
      "\xe1", "\xe2", "\xe3", "\xe4", "\xe5", "\xe6", "\xe7", "\xe8", "\xe9",
      "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef", "\xf0", "\xf1", "\xf2",
      "\xf3", "\xf4", "\xf5", "\xf6", "\xf7", "\xf8", "\xf9", "\xfa", "\xfb",
      "\xfc", "\xfd", "\xfe", "\xff",
  };
  memcpy(table, raw_table, sizeof raw_table);
}

void cat_dollar_at_the_end(const char *table[static 256]) {
  table['\n'] = "$\n";
}

void cat_tabs(const char *table[static 256]) { table['\t'] = "^I"; }

void cat_non_printed(const char *table[static 256]) {
  const char *example_1[] = {"^@", "^A", "^B", "^C", "^D",
                             "^E", "^F", "^G", "^H"};
  const char *example_2[] = {"^K", "^L", "^M", "^N",  "^O", "^P", "^Q",
                             "^R", "^S", "^T", "^U",  "^V", "^W", "^X",
                             "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"};
  const char *example_3[] = {
      "^?",   "M-^@", "M-^A",  "M-^B", "M-^C", "M-^D", "M-^E", "M-^F", "M-^G",
      "M-^H", "M-^I", "M-^J",  "M-^K", "M-^L", "M-^M", "M-^N", "M-^O", "M-^P",
      "M-^Q", "M-^R", "M-^S",  "M-^T", "M-^U", "M-^V", "M-^W", "M-^X", "M-^Y",
      "M-^Z", "M-^[", "M-^\\", "M-^]", "M-^^", "M-^_", "M- ",  "M-!",  "M-\"",
      "M-#",  "M-$",  "M-%",   "M-&",  "M-'",  "M-(",  "M-)",  "M-*",  "M-+",
      "M-,",  "M--",  "M-.",   "M-/",  "M-0",  "M-1",  "M-2",  "M-3",  "M-4",
      "M-5",  "M-6",  "M-7",   "M-8",  "M-9",  "M-:",  "M-;",  "M-<",  "M-=",
      "M->",  "M-?",  "M-@",   "M-A",  "M-B",  "M-C",  "M-D",  "M-E",  "M-F",
      "M-G",  "M-H",  "M-I",   "M-J",  "M-K",  "M-L",  "M-M",  "M-N",  "M-O",
      "M-P",  "M-Q",  "M-R",   "M-S",  "M-T",  "M-U",  "M-V",  "M-W",  "M-X",
      "M-Y",  "M-Z",  "M-[",   "M-\\", "M-]",  "M-^",  "M-_",  "M-`",  "M-a",
      "M-b",  "M-c",  "M-d",   "M-e",  "M-f",  "M-g",  "M-h",  "M-i",  "M-j",
      "M-k",  "M-l",  "M-m",   "M-n",  "M-o",  "M-p",  "M-q",  "M-r",  "M-s",
      "M-t",  "M-u",  "M-v",   "M-w",  "M-x",  "M-y",  "M-z",  "M-{",  "M-|",
      "M-}",  "M-~",  "M-^?"};

  memcpy(table, example_1, sizeof example_1);
  memcpy(&table[9 + 2], example_2, sizeof example_2);
  memcpy(&table['~' + 1], example_3, sizeof example_3);
}

void cat_print_file(FILE *file, Flags flags, const char *table[static 256]) {
  int a = 0;
  int lst = '\n';
  int linenumber = 0;
  bool sqviz = false;
  while (fread(&a, 1, 1, file) > 0) {
    if (lst == '\n') {
      if (flags.compression && a == '\n') {
        if (sqviz) continue;
        sqviz = true;
      } else
        sqviz = false;

      if (flags.no_empty_strings) {
        if (a != '\n') printf("%6i\t", ++linenumber);
      } else if (flags.numbering_all) {
        printf("%6i\t", ++linenumber);
      }
    }
    if (!*table[a]) {
      printf("%c", '\0');
    } else {
      printf("%s", table[a]);
    }
    lst = a;
  }
}

void cat_open_file_or_stdin(int argc, char *argv[], Flags flags,
                            const char *table[static 256]) {
  int count_flags = 0;
  for (char **filename = &argv[1], **end = &argv[argc]; filename != end;
       ++filename) {
    if (**filename == '-') {
      count_flags++;
      continue;
    }
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    cat_print_file(file, flags, table);
    fclose(file);
  }
  if (argc == count_flags + 1) {
    cat_print_file(stdin, flags, table);
  }
}
