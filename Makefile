CC = clang
CFLAGS = -lncurses  -pthread

all: memory.o cpu.o sysmon.o disk.o procs.o
	$(CC) $(CFLAGS) -o sysmon obj/memory.o obj/cpu.o obj/sysmon.o obj/disk.o obj/procs.o

memory.o:
	$(CC) $(CFLAGS) -c src/memory.c -o obj/memory.o
cpu.o:
	$(CC) $(CFLAGS) -c src/cpu.c -o obj/cpu.o
disk.o:
	$(CC) $(CFLAGS) -c src/disk.c -o obj/disk.o
procs.o:
	$(CC) $(CFLAGS) -c src/procs.c -o obj/procs.o
sysmon.o:
	$(CC) $(CFLAGS) -c src/sysmon.c -o obj/sysmon.o
clean:
	rm -f sysmon obj/*.o
