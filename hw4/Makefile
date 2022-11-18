#Based on provided Makefile
CC = gcc
CFLAGS = -Wall -g #replace -g with -O when not debugging

DEPS	= queue.h Makefile
OBJS	= myscheduler.o queue.o

EXECS	= myscheduler

all:	$(EXECS)

%.o:	%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

myscheduler:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: $(all)
	gdb -tui search

clean:
	/bin/rm -i *.o $(EXECS)
