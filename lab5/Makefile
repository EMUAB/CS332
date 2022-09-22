# Based off of the previously supplied makefile

CC = gcc
CFLAGS = -Wall -g

DEPS	= Makefile
OBJS	= lab5.o

EXECS	= lab5

all:	$(EXECS)

%.o:	%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

insertionsort:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:	
	/bin/rm -i *.o $(EXECS)
