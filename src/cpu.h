#ifndef CPU_H
#define CPU_H

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
