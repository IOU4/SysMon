#include "procs.h"
#include <ctype.h>
#include <curses.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void print_procs() {
  WINDOW *procs_win = newwin(LINES - 2, COLS - 2, 2, 1);
  int pids_count;
  uint *pids = get_running_pids(&pids_count);
  box(procs_win, 0, 0);
  mvwprintw(procs_win, 1, 1, "pid");
  mvwprintw(procs_win, 1, 12, "cpu");
  mvwprintw(procs_win, 1, 24, "memory");
  for (int i = 0; i < pids_count; i++) {
    Process proccess = get_proccess_info(pids[i]);
    mvwprintw(procs_win, i + 3, 1, "%d", proccess.pid);
    mvwprintw(procs_win, i + 3, 12, "%.4f %%", proccess.cpu_usage);
    mvwprintw(procs_win, i + 3, 24, "%.4f Mb ", proccess.memory_usage);
  }
  wrefresh(procs_win);
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
