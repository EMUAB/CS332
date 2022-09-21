# Sample Makefile to compile C programs

CC = gcc
CFLAGS = -Wall -g #replace -g with -O when not debugging

DEPS	= gettime.h Makefile
OBJS	= gettime.o insertionsort.o

EXECS	= insertionsort

all:	$(EXECS)

%.o:	%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

insertionsort:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:	
	/bin/rm -i *.o $(EXECS)
