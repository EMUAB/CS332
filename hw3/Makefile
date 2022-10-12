CC = gcc
CFLAGS = -Wall -g #replace -g with -O when not debugging
DEPS	= Makefile
OBJS	= search.o
EXECS	= search

all:	$(EXECS)

%.o:	%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

search:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	/bin/rm -i *.o $(EXECS)
