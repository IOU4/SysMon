#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"

void print_memory() {
  WINDOW *memroy_win = newwin(WIN_HEIGHT, WIN_WIDTH, 7, 1);
  box(memroy_win, 0, 0);
  mvwprintw(memroy_win, 0, 2, "MEMORY:");
  mvwprintw(memroy_win, 1, 1, "total: %.1f Mb", parse_mem_info()[0]);
  mvwprintw(memroy_win, 2, 1, "free: %.1f Mb", parse_mem_info()[1]);
  mvwprintw(memroy_win, 3, 1, "available: %.1f Mb", parse_mem_info()[2]);
  wrefresh(memroy_win);
}

float *parse_mem_info() {
  FILE *mem_file = fopen("/proc/meminfo", "r");
  if (mem_file == NULL) {
    perror("couldn't open /proc/meminfo");
    exit(1);
  }

  float *res = malloc(2 * sizeof(unsigned long));
  char line[MAX_LINE_LENGTH];

  while (fgets(line, MAX_LINE_LENGTH, mem_file)) {
    int found = 0;
    if (strncmp("MemTotal:", line, 8) == 0) {
      res[0] = kb_to_mb(get_value_from_memory_line(line));
      found++;
    }
    if (strncmp("MemFree:", line, 7) == 0) {
      res[1] = kb_to_mb(get_value_from_memory_line(line));
      found++;
    }
    if (strncmp("MemAvailable:", line, 12) == 0) {
      res[2] = kb_to_mb(get_value_from_memory_line(line));
      found++;
    }
    if (found >= 3)
      return res;
  }
  fclose(mem_file);
  return res;
}

unsigned long get_value_from_memory_line(char *line) {
  unsigned long value;
  sscanf(line, "%*s %lu", &value);
  return value;
}

float kb_to_mb(unsigned long kb) { return (kb * 1.0) / 1024; }
