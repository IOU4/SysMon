#include <curses.h>
#include <ncurses.h>
#include <stdio.h>

#include "memory.h"

int main() {

  initscr();
  noecho();
  curs_set(0);
  border(0, 0, 0, 0, 0, 0, 0, 0);
  // WINDOW *memory_win = newwin(10, 20, 5, 15);
  // mvwprintw(memory_win, 8, 8, "%lo", get_total_memory());
  refresh();
  memory();
  while (true) {
  }

  endwin();
  return 0;
}
