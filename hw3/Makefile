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

debug: $(all)
	gdb -tui search

clean:
	/bin/rm -i *.o $(EXECS)
