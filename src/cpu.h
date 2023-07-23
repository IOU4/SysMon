#ifndef CPU_H
#define CPU_H

#define MAX_LINE_LENGTH 255
#define WIN_WIDTH 34
#define WIN_HEIGHT 5

#include <stdio.h>
struct cpustat {
  unsigned long user;
  unsigned long nice;
  unsigned long sys;
  unsigned long idle;
  unsigned long iowait;
  unsigned long irq;
  unsigned long softirq;
} typedef cpu_stat;

void print_cpu();
cpu_stat get_value_from_cpu_line(char *);
void read_stats(cpu_stat *);
float calculate_usage();
void print_cpu_stat(cpu_stat stat);
#endif
