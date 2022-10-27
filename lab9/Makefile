# Based off of the previously supplied makefile

CC = gcc
CFLAGS = -Wall -g

DEPS	= Makefile
OBJS	= lab9.o

EXECS	= lab9

all:	$(EXECS)

%.o:	%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

lab9:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: $(all)
	gdb -tui lab9

clean:	
	/bin/rm -i *.o $(EXECS)
