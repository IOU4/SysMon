#include <curses.h>
#include <ncurses.h>
#include <stdio.h>

#include "memory.h"

int main() {

  initscr();
  noecho();
  curs_set(0);
  border(0, 0, 0, 0, 0, 0, 0, 0);
  refresh();
  print_memory();
  while (true) {
  }
  getch();
  endwin();
  return 0;
}
