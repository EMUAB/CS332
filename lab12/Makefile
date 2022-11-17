# Based off of the previously supplied makefile

CC = gcc
CFLAGS = -Wall -O -lpthread

DEPS	= Makefile

EXECS	= lab12

all:	$(EXECS)

lab12:
	$(CC) lab12.c $(CFLAGS) -o $@ $^

debug: $(all)
	gdb -tui lab11

clean:	
	/bin/rm -i *.o $(EXECS)
