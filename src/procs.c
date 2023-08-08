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

void print_procs() {
  int pids_count;
  uint *pids = get_running_pids(&pids_count);
  WINDOW *procs_win = newwin(pids_count + 2, COLS - 2, 5, 1);
  scrollok(procs_win, true);
  mvprintw(3, 1, "pid");
  mvprintw(3, 12, "cpu");
  mvprintw(3, 24, "memory");
  pthread_t read_key_thread;
  Thread_data data;
  data.procs_win = procs_win;
  data.current_row = 0;
  pthread_create(&read_key_thread, NULL, scroll_window, &data);
  while (1) {
    pids = get_running_pids(&pids_count);
    for (int i = 0; i < pids_count; i++) {
      Process proccess = get_proccess_info(pids[i]);
      mvwprintw(procs_win, i + 1, 1, "%d", proccess.pid);
      mvwprintw(procs_win, i + 1, 12, "%.4f %%", proccess.cpu_usage);
      mvwprintw(procs_win, i + 1, 24, "%.4f Mb", proccess.memory_usage);
    }
    for (int k = data.current_row; k > 0; k--)
      scroll(procs_win);
    wrefresh(procs_win);
    napms(5000);
  }
}

void *scroll_window(void *arg) {
  Thread_data *data = (Thread_data *)arg;
  int ch;
  while ((ch = getch()) != 'q') {
    if (ch == KEY_DOWN) {
      (data->current_row)++;
      scroll(data->procs_win);
    }
    wrefresh(data->procs_win);
    napms(100);
  }
  return NULL;
}

uint *get_running_pids(int *pids_count) {
  DIR *dir;
  struct dirent *entry;
  uint static pids[1000];
  *pids_count = 0;
  if ((dir = opendir("/proc")) == NULL) {
    perror("Failed to open /proc");
    return 0;
  }
  while ((entry = readdir(dir))) {
    if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
      pids[*pids_count] = atoi(entry->d_name);
      (*pids_count)++;
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
    exit(1);
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
