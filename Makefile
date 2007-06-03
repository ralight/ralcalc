CC=gcc
CFLAGS=-ggdb -Wall -O2
LDFLAGS=-nopie -lm

all : ccalc

ccalc : ccalc.o list.o output.o
	$(CC) $(LDFLAGS) -o $@ $^

ccalc.o : ccalc.c ccalc.h datatypes.h
	$(CC) ${CFLAGS} -c -o $@ $<

list.o : list.c list.h datatypes.h
	$(CC) $(CFLAGS) -c -o $@ $<

output.o : output.c output.h datatypes.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	-rm -f ccalc *.o

install : ccalc
	install --strip ccalc /usr/local/bin/=

uninstall :
	rm -f /usr/local/bin/= core

