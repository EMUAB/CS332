# Based off of the previously supplied makefile

CC = gcc
CFLAGS = -Wall -g

DEPS	= Makefile
OBJS	= lab7.o

EXECS	= lab7

all:	$(EXECS)

%.o:	%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

lab7:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: $(all)
	gdb -tui lab7

clean:	
	/bin/rm -i *.o $(EXECS)
