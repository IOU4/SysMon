#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#include "cpu.h"
#include "memory.h"

int main() {

  initscr();
  noecho();
  curs_set(0);
  refresh();
  while (1) {
    print_memory();
    print_cpu();
    sleep(1);
  }
  getch();
  endwin();
  return 0;
}
