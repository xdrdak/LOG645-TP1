PROG = tp1
CFLAGS = -g -fPIC -m64 -Wall
LFLAGS= -fPIC -m64 -Wall
CC = clang

all: $(PROG)

tp1.o: tp1.c
	$(CC) $(CFLAGS) -c tp1.c

tp1 : tp1.o
	$(CC) $(LFLAGS) tp1.o -o tp1

run_1:
	./tp1 1 2 3

ps:
	ps -fu $$USER

clean:
	/bin/rm -f *~
	/bin/rm -f *.o
