#include <curses.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>

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
  pthread_t procs_thred;
  pthread_create(&procs_thred, NULL, print_procs, NULL);
  while (1) {
    mvprintw(1, 1, "Overall stats:");
    refresh();
    print_memory();
    print_disk();
    print_cpu(&is_first_run);
    refresh();
    napms(1000);
  }
  endwin();
  return 0;
}
