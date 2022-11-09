# Based off of the previously supplied makefile

CC = gcc
CFLAGS = -Wall -g

DEPS	= Makefile
OBJS	= lab11.o

EXECS	= lab11

all:	$(EXECS)

%.o:	%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

lab7:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: $(all)
	gdb -tui lab11

clean:	
	/bin/rm -i *.o $(EXECS)
