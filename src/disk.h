#ifndef DISK_H
#define DISK_H
#define DISK_MAX_LINE_LENGTH 255
#define DISK_WIN_WIDTH COLS - 2
#define DISK_WIN_HEIGHT 10
#define MAX_DISK_NAME_LEN 25
#define MAX_DISK_PARTIONS 25
#define MAX_DEVICES 10
#include <stdbool.h>

typedef struct device {
  int major;
  int minor;
  char name[MAX_DISK_NAME_LEN];
  unsigned long long sectors_read;
  unsigned long long read_time_ms;
  unsigned long long sectors_written;
  unsigned long long write_time_ms;
} Device;

typedef struct partition {
  char name[MAX_DISK_NAME_LEN];
  float write_rate;
  float read_rate;
} partition;

typedef struct disk {
  char name[MAX_DISK_NAME_LEN];
  float write_rate;
  float read_rate;
  int partitions_count;
  partition partitions[MAX_DISK_PARTIONS];
} Disk;

void print_disk();
Disk *get_disks(int *disk_count);
bool array_contains(int *, int, int);
Device *parse_stats_file(int *device_count);

void print_device(Device);
#endif
