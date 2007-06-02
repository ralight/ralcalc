CC=gcc
CFLAGS=-ggdb -Wall
LDFLAGS=-nopie -lm

all : ccalc

ccalc : ccalc.o list.o
	$(CC) $(LDFLAGS) -o $@ $^

ccalc.o : ccalc.c ccalc.h
	$(CC) ${CFLAGS} -c -o $@ $<

list.o : list.c list.h ccalc.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	-rm -f ccalc *.o

install : ccalc
	install --strip ccalc /usr/local/bin/=

uninstall :
	rm -f /usr/local/bin/= core
