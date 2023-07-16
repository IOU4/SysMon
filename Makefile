CC = clang
CFLAGS = -lncurses 

all: memory.o sysmon.o
	$(CC) $(CFLAGS) -o sysmon obj/memory.o obj/sysmon.o 

memory.o:
	$(CC) $(CFLAGS) -c src/memory.c -o obj/memory.o
sysmon.o:
	$(CC) $(CFLAGS) -c src/sysmon.c -o obj/sysmon.o
clean:
	rm -f sysmon obj/*.o
