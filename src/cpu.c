#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cpu.h"

void print_cpu() {
  WINDOW *cpu_win = newwin(WIN_HEIGHT, WIN_WIDTH, 2, 1);
  box(cpu_win, 0, 0);
  mvwprintw(cpu_win, 0, 2, "CPU:");
  float total_usage = calculate_usage();
  mvwprintw(cpu_win, 1, 1, "overall usage: %.4f %%", total_usage);
  mvwprintw(cpu_win, 2, 1, "overall free: %.4f %%", 100 - total_usage);
  wrefresh(cpu_win);
}

void read_stats(cpu_stat *stats) {
  FILE *stat_file = fopen("/proc/stat", "r");
  char line[MAX_LINE_LENGTH];

  while (fgets(line, MAX_LINE_LENGTH, stat_file)) {
    if (sscanf(line, "cpu %lu %lu %lu %lu %lu %lu %lu", &(stats->user),
               &(stats->nice), &(stats->sys), &(stats->idle), &(stats->iowait),
               &(stats->irq), &(stats->softirq)))
      break;
  }
  fclose(stat_file);
}

float calculate_usage() {
  cpu_stat start_stat, end_stat;

  read_stats(&start_stat);
  usleep(500000);
  read_stats(&end_stat);

  // Calculate the difference in CPU usage
  unsigned long total_cpu_time_start =
      start_stat.user + start_stat.nice + start_stat.sys + start_stat.idle +
      start_stat.iowait + start_stat.irq + start_stat.softirq;
  unsigned long total_cpu_time_end =
      end_stat.user + end_stat.nice + end_stat.sys + end_stat.idle +
      end_stat.iowait + end_stat.irq + end_stat.softirq;
  unsigned long total_cpu_time_diff = total_cpu_time_end - total_cpu_time_start;

  unsigned long total_idle_start = start_stat.idle;
  unsigned long total_idle_end = end_stat.idle;
  unsigned long total_idle_diff = total_idle_end - total_idle_start;

  // Calculate total CPU usage as a percentage
  float cpu_usage_percentage =
      100.0 * (total_cpu_time_diff - total_idle_diff) / total_cpu_time_diff;

  return cpu_usage_percentage;
}

void print_cpu_stat(cpu_stat stat) {
  mvprintw(11, 1, "User:     %lu\n", stat.user);
  mvprintw(12, 1, "Nice:     %lu\n", stat.nice);
  mvprintw(13, 1, "System:   %lu\n", stat.sys);
  mvprintw(14, 1, "Idle:     %lu\n", stat.idle);
  refresh();
}
