#include <curses.h>
#include <ncurses.h>
#include <sys/resource.h>

#include "memory.h"

void memory() {
  WINDOW *memroy_win = newwin(10, 20, 5, 15);
  box(memroy_win, 0, 0);
  mvwprintw(memroy_win, 2, 2, "%lo", get_total_memory());
  wrefresh(memroy_win);
}

long get_total_memory() {
  struct rusage r_usage;
  getrusage(RUSAGE_SELF, &r_usage);
  return r_usage.ru_maxrss;
}
