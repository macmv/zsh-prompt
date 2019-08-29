#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct Section {
  char* text;
  char* fg;
  char* bg;
};

char* get_rgb(char* code) {
  if (strlen(code) != 3) {
    printf("String must be 3 chars!");
    exit(1);
  }
  char* str = malloc(sizeof(char) * 11); // because the color is eleven chars; example: 255;255;255
  int color = (int) strtol(code, NULL, 16);
  int r = (color / 256) * 17;
  int g = (color / 16 % 16) * 17;
  int b = (color % 16) * 17;
  sprintf(str, "%d;%d;%d", r, g, b);
  return str;
}

char* paint_with_spacing(char* text, char* fg, char* bg, bool direction) {
  char* str = malloc(sizeof(char) * 50);
  strcpy(str, "\e[38;2;");
  strcat(str, get_rgb(fg));
  strcat(str, ";48;2;");
  strcat(str, get_rgb(bg));
  strcat(str, "m");
  if (direction) {
    strcat(str, " ");
    strcat(str, text);
  } else {
    strcat(str, text);
    strcat(str, " ");
  }
  strcat(str, "\e[38;2;255;255;255;48;2;0;0;0m");
  return str;
}


char* paint(char* text, char* fg, char* bg) {
  char* str = malloc(sizeof(char) * 50);
  strcpy(str, "\e[38;2;");
  strcat(str, get_rgb(fg));
  strcat(str, ";48;2;");
  strcat(str, get_rgb(bg));
  strcat(str, "m");
  strcat(str, text);
  strcat(str, "\e[38;2;255;255;255;48;2;0;0;0m");
  return str;
}

/*
separator: the triangle to seperate sections
sections: array of strings and colors to join
length: length of sections
direction:
  true: is the prompt on the right
  false: is the prompt on the left
  this changes how the colors should change with the separators
*/
char* generate_sections(char* separator, struct Section sections[], int length, bool direction) {
  char* str = malloc(length * sizeof(char) * 50);
  strcpy(str, "\0");
  for (int i = 0; i < length; i++) {
    struct Section sec1;
    struct Section sec2;
    if (direction) {
      if (i <= 0) {
        sec1.text = separator;
        sec1.fg = sections[i].bg;
        sec1.bg = "000";
      } else {
        sec1.text = separator;
        sec1.fg = sections[i].bg;
        sec1.bg = sections[i - 1].bg;
      }
      sec2 = sections[i];
    } else {
      sec1 = sections[i];
      if (i >= length - 1) {
        sec2.text = separator;
        sec2.fg = sections[i].bg;
        sec2.bg = "000";
      } else {
        sec2.text = separator;
        sec2.fg = sections[i].bg;
        sec2.bg = sections[i + 1].bg;
      }
    }
    strcat(str, paint_with_spacing(sec1.text, sec1.fg, sec1.bg, direction));
    strcat(str, paint_with_spacing(sec2.text, sec2.fg, sec2.bg, direction));
  }
  return str;
}
