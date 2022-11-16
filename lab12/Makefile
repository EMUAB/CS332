# Based off of the previously supplied makefile

CC = gcc
CFLAGS = -Wall -g -O -lpthread

DEPS	= Makefile
OBJS	= lab12.o

EXECS	= lab12

all:	$(EXECS)

%.o:	%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

lab12:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: $(all)
	gdb -tui lab11

clean:	
	/bin/rm -i *.o $(EXECS)
