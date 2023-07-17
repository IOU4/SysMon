#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#include "memory.h"

#define MAX_LENGTH 40

struct rusage r_usage;

void print_memory() {
  WINDOW *memroy_win = newwin(10, 20, 5, 15);
  getrusage(RUSAGE_SELF, &r_usage);
  box(memroy_win, 0, 0);
  mvwprintw(memroy_win, 1, 1, "memory");
  mvwprintw(memroy_win, 3, 1, "total: %ld kbytes", parse_mem_info()[0]);
  wrefresh(memroy_win);
}

unsigned long *parse_mem_info() {
  FILE *mem_file = fopen("/proc/meminfo", "r");
  if (mem_file == NULL) {
    perror("couldn't open /proc/meminfo");
    exit(1);
  }

  unsigned long *res = malloc(2 * sizeof(long));
  char line[MAX_LENGTH];

  while (fgets(line, MAX_LENGTH, mem_file)) {
    int found = 0;
    if (strncmp("MemTotal:", line, 8) == 0) {
      res[0] = get_value_from_line(line);
      found++;
    }
    if (strncmp("MemFree:", line, 8) == 0) {
      res[1] = get_value_from_line(line);
      found++;
    }
    if (found >= 2)
      return res;
  }
  return res;
}

unsigned long get_value_from_line(char *line) {
  unsigned long value;
  sscanf(line, "%*s %lu", &value);
  return value;
}
