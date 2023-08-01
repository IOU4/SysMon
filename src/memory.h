#ifndef MEMORY_H
#define MEMORY_H

#define MAX_LINE_LENGTH 255
#define WIN_WIDTH COLS - 2
#define WIN_HEIGHT 5

void print_memory();
float *parse_mem_info();
unsigned long get_value_from_memory_line(char *);
float kb_to_mb(unsigned long);

#endif
