#include "procs.h"
#include <ctype.h>
#include <curses.h>
#include <dirent.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

uint *pids;
int procs_count;
Process *procs;

void *print_procs(void *args) {
  get_running_pids();
  WINDOW *procs_win =
      newwin(procs_count + 2, PROCS_WIN_WIDTH, 3, PROCS_WIN_WIDTH);
  scrollok(procs_win, true);
  print_window_info(procs_win);
  pthread_t read_key_thread;
  Thread_data data;
  data.procs_win = procs_win;
  data.current_row = 0;
  pthread_create(&read_key_thread, NULL, scroll_window, &data);
  while (1) {
    print_window_info(procs_win);
    pids = get_running_pids();
    procs = malloc(procs_count * sizeof(Process));
    for (int i = 0; i < procs_count; i++)
      procs[i] = get_proccess_info(pids[i]);
    wclear(procs_win);
    show_procs(procs_win);
    for (int k = data.current_row; k > 0; k--)
      scroll(procs_win);
    wrefresh(procs_win);
    napms(5000);
    free(procs);
  }
  return NULL;
}

void *scroll_window(void *arg) {
  Thread_data *data = (Thread_data *)arg;
  int ch;
  while ((ch = getch()) != 'q') {
    if (ch == KEY_DOWN && data->current_row < procs_count) {
      (data->current_row)++;
      wscrl(data->procs_win, 1);
    } else if (ch == KEY_UP && (data->current_row) > 0) {
      (data->current_row)--;
      wclear(data->procs_win);
      show_procs(data->procs_win);
      for (int i = 0; i < data->current_row; i++)
        scroll(data->procs_win);
    }
    wrefresh(data->procs_win);
    napms(1);
  }
  return NULL;
}

uint *get_running_pids() {
  DIR *dir;
  struct dirent *entry;
  uint static pids[1000];
  procs_count = 0;
  if ((dir = opendir("/proc")) == NULL) {
    perror("Failed to open /proc");
    return 0;
  }
  while ((entry = readdir(dir))) {
    if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
      pids[procs_count] = atoi(entry->d_name);
      procs_count++;
    }
  }
  closedir(dir);
  return pids;
}

Process get_proccess_info(uint pid) {
  Process proccess;
  proccess.pid = pid;
  char path_to_stats[255];
  char line[255];
  sprintf(path_to_stats, "/proc/%d/stat", pid);
  FILE *stats_file = fopen(path_to_stats, "r");
  if (stats_file == NULL) {
    char erorr_msg[255];
    sprintf(erorr_msg, "couldn't read %s", path_to_stats);
    perror(erorr_msg);
  }

  if (fgets(line, 255, stats_file) != NULL) {
    ulong stime, utime, start_time, rss;
    sscanf(line,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d "
           "%*d %*d %*d %*d %*d %lu %*lu %lu",
           &utime, &stime, &start_time, &rss);
    ulong elpased_time_seconds =
        (get_uptime_ticks() + start_time) / sysconf(_SC_CLK_TCK);
    ulong process_time_seconds = (float)(utime + stime) / sysconf(_SC_CLK_TCK);
    proccess.cpu_usage = 1.0 * process_time_seconds / elpased_time_seconds;
    proccess.memory_usage = 1.0 * rss * sysconf(_SC_PAGESIZE) / (1024 * 1024);
  }
  fclose(stats_file);
  return proccess;
}

float get_uptime_ticks() {
  FILE *uptime_file = fopen("/proc/uptime", "r");
  if (uptime_file == NULL) {
    perror("could't read '/proc/uptime'");
    return 0;
  }
  char line[25];
  float uptime = 0;
  fscanf(uptime_file, "%f", &uptime);
  fclose(uptime_file);
  return uptime;
}

void show_procs(WINDOW *procs_win) {
  for (int i = 0; i < procs_count; i++) {
    procs[i] = get_proccess_info(pids[i]);
    mvwprintw(procs_win, i, 1, "%d", procs[i].pid);
    mvwprintw(procs_win, i, 12, "%.4f %%", procs[i].cpu_usage);
    mvwprintw(procs_win, i, 24, "%.4f Mb", procs[i].memory_usage);
  }
}

void print_window_info(WINDOW *procs_win) {
  box(procs_win, 0, 0);
  mvprintw(1, PROCS_WIN_WIDTH, "Processes:");
  mvprintw(1, PROCS_WIN_WIDTH + 12, "total: %d", procs_count);
  mvprintw(1, PROCS_WIN_WIDTH + 24, "_use arrow keys to scroll_");
  mvprintw(2, PROCS_WIN_WIDTH + 1, "PID:");
  mvprintw(2, PROCS_WIN_WIDTH + 12, "CPU:");
  mvprintw(2, PROCS_WIN_WIDTH + 24, "MEMORY:");
}
