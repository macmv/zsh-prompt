#include "sections.c"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <wchar.h>
#include <unistd.h>
#include <locale.h>
#include <limits.h>

char* show_bin(char c) {
  char* str = malloc(8 * sizeof(char));
  for (int i = 7; i >= 0; i--) {
    if (c & (1 << i)) {
      str[i] = '1';
    } else {
      str[i] = '0';
    }
  }
  return str;
}

int utf8_length(char* str) {
  int length = 0;
  for (int i = 0; i < 1000; i++) {
    char let = str[i];
    if (let == '\0') {
      break;
    }
    if (let >> 6 == -2) {
      continue;
    }
    // skip until hit 'm'
    if (let == '\e') {
      int j;
      for (j = 0; j < 1000; j++) {
        let = str[i + j];
        if (let == '\0') {
          j--;
          break;
        }
        if (let == 'm') {
          break;
        }
      }
      i += j;
    } else {
      length += 1;
    }
  }
  return length;
}

int main(int argc, char *argv[]) {
  setlocale(LC_CTYPE, "");
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror("getcwd() error");
    return 1;
  }
  struct Section left_sections[] = {
    (struct Section) {.text = " macmv", .fg = "fff", .bg = "06a"}, // macmv
    (struct Section) {.text = "neils-pc", .fg = "fff", .bg = "09d"}, // neils-pc
    (struct Section) {.text = cwd, .fg = "fff", .bg = "555"}, // current dir
    (struct Section) {.text = "git status", .fg = "fff", .bg = "2a2"}, // git status
  };
  struct Section right_sections[] = {
    (struct Section) {.text = "time", .fg = "000", .bg = "ccc"}, // time
    (struct Section) {.text = "date with letters", .fg = "fff", .bg = "888"}, // date but letters
    (struct Section) {.text = "date with numbers", .fg = "fff", .bg = "555"}, // date but numbers
  };
  if (argc != 2) {
    printf("Must enter width as first arg");
    return 1;
  }
  int width = atoi(argv[1]);
  char* left_string = generate_sections("\uE0B0", left_sections, 4, false);
  char* right_string = generate_sections("\uE0B2", right_sections, 3, true);
  printf(paint("┏━", "06a", "000"));
  printf(left_string);
  int left_length = 2 + utf8_length(left_string);
  int right_length = utf8_length(right_string);
  for (int i = 0; i < width - left_length - right_length; i++) {
    printf(" ");
  }
  printf(right_string);
  printf("\n");
  printf(paint("┗━ ", "06a", "000"));

  return 0;
}
