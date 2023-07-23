#include <curses.h>
#include <ncurses.h>
#include <stdio.h>

#include "cpu.h"
#include "memory.h"

int main() {

  initscr();
  noecho();
  curs_set(0);
  border(0, 0, 0, 0, 0, 0, 0, 0);
  refresh();
  // print_memory();
  print_cpu();
  getch();
  endwin();
  return 0;
}
