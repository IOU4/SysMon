#ifndef PROCSS_H
#define PROCSS_H

#include <curses.h>
#include <sys/types.h>
#define MAX_LINE_LENGTH 255
#define PROCS_WIN_WIDTH COLS - 2
#define WIN_HEIGHT 5

typedef struct procces_info {
  uint pid;
  float memory_usage;
  float cpu_usage;
} Process;

typedef struct thread_data {
  int current_row;
  WINDOW *procs_win;
} Thread_data;

void print_procs();
uint *get_running_pids(int *pids_counts);
Process get_proccess_info(uint pid);
float get_uptime_ticks();
void *scroll_window(void *arg);

#endif
