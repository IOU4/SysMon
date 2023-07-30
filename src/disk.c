#include "disk.h"
#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_disk() {

  int win_height = 0, disk_count, last_height = 1;
  Disk *disks = get_disks(&disk_count);
  for (int i = 0; i < disk_count; i++)
    win_height += 3 + disks[i].partitions_count;
  WINDOW *disk_win = newwin(win_height, DISK_WIN_WIDTH, 13, 1);
  box(disk_win, 0, 0);
  mvwprintw(disk_win, 0, 2, " Disks: ");
  for (int i = 0; i < disk_count; i++) {
    mvwprintw(disk_win, last_height++, 1,
              "%s: Read: %.2f M/sec, Write: %.2f M/sec", disks[i].name,
              disks[i].read_rate, disks[i].write_rate);
    for (int j = 0; j < disks[i].partitions_count; j++) {
      mvwprintw(disk_win, last_height++, 4,
                "%s: Read: %.2f M/sec, Write: %.2f M/sec",
                disks[i].partitions[j].name, disks[i].partitions[j].read_rate,
                disks[i].partitions[j].write_rate);
    }
  }
  wrefresh(disk_win);
};

Disk *get_disks(int *disk_count) {
  Disk static disks[5];
  int device_count;
  Device *devices = parse_stats_file(&device_count);
  *disk_count = 0;
  int seen_majors[device_count];
  int last_seen_major = 0;

  for (int i = 0; i < device_count; i++) {
    Device device = devices[i];
    if (array_contains(seen_majors, device_count, device.major)) {
      partition part;
      strcpy(part.name, device.name);
      part.read_rate = device.read_time_ms == 0
                           ? 0.0
                           : 0.512 * device.sectors_read / device.read_time_ms;
      part.write_rate =
          device.write_time_ms == 0
              ? 0.0
              : 0.512 * device.sectors_written / device.write_time_ms;
      disks[(*disk_count) - 1].partitions[device.minor - 1] = part;
      disks[(*disk_count) - 1].partitions_count++;
    } else {
      Disk disk;
      strcpy(disk.name, device.name);
      disk.read_rate = device.read_time_ms == 0
                           ? 0.0
                           : 0.512 * device.sectors_read / device.read_time_ms;
      disk.write_rate =
          device.write_time_ms == 0
              ? 0.0
              : 0.512 * device.sectors_written / device.write_time_ms;
      disk.partitions_count = 0;
      disks[*disk_count] = disk;
      (*disk_count)++;
      seen_majors[last_seen_major++] = device.major;
    }
  }
  return disks;
}

Device *parse_stats_file(int *device_count) {
  Device static devices[MAX_DEVICES];
  *device_count = 0;
  FILE *disk_file = fopen("/proc/diskstats", "r");
  char buffer[DISK_MAX_LINE_LENGTH];
  while (fgets(buffer, sizeof(buffer), disk_file) != NULL &&
         *device_count < MAX_DEVICES) {
    int ret = sscanf(
        buffer, "%d %d %31s %*llu %*llu %llu %llu %*llu %*llu %llu %llu",
        &devices[*device_count].major, &devices[*device_count].minor,
        devices[*device_count].name, &devices[*device_count].sectors_read,
        &devices[*device_count].read_time_ms,
        &devices[*device_count].sectors_written,
        &devices[*device_count].write_time_ms);
    if (ret != 7) {
      fprintf(stderr, "Error parsing line: %s", buffer);
      continue;
    }
    (*device_count)++;
  }
  fclose(disk_file);
  return devices;
}

bool array_contains(int *array, int length, int target) {
  for (int i = 0; i < length; i++) {
    if (array[i] == target)
      return true;
  }
  return false;
}

void print_device(Device device) {
  int i = 25;
  mvprintw(i + 0, 1, "major: %d", device.major);
  mvprintw(i + 1, 1, "minor: %d", device.minor);
  mvprintw(i + 2, 1, "sector read: %llull", device.sectors_read);
  mvprintw(i + 3, 1, "read time: %llull", device.read_time_ms);
  mvprintw(i + 4, 1, "sector written: %llull", device.sectors_written);
  mvprintw(i + 5, 1, "write time: %llull", device.write_time_ms);
  refresh();
}
