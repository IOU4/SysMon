CC=clang

all: 
	$(CC) -o sysmon src/sysmon.c
clean:
	rm sysmon
