
PROG = calltest
OBJS = calltest.o call.o

CFLAGS = -Wall -pedantic -std=c99 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make -j $(nproc)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

calltest.o: call.h
call.o: call.h inode.h superblock.h

.PHONY: clean

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f $(PROG)

