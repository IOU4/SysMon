#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>

#include "cpu.h"
#include "disk.h"
#include "memory.h"
#include "procs.h"

int main() {

  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  bool is_first_run = true;
  mvprintw(1, 1, "Overall stats: ");
  refresh();
  // int i = 0;
  // while (1) {
  //   print_memory();
  //   print_disk();
  //   print_cpu(&is_first_run);
  print_procs();
  // i++;
  // sleep(1);
  // }
  getch();
  endwin();
  return 0;
}
