CC=gcc
CFLAGS=-ggdb -Wall -O2
LDFLAGS=-nopie -lm
VERSION=20070603

all : ccalc

ccalc : calculation.o ccalc.o output.o tokens.o
	$(CC) $(LDFLAGS) -o $@ $^

calculation.o : calculation.c calculation.h datatypes.h
	$(CC) ${CFLAGS} -c -o $@ $<

ccalc.o : ccalc.c ccalc.h datatypes.h
	$(CC) -DVERSION=\"${VERSION}\" ${CFLAGS} -c -o $@ $<

output.o : output.c output.h datatypes.h
	$(CC) $(CFLAGS) -c -o $@ $<

tokens.o : tokens.c tokens.h datatypes.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	-rm -f ccalc *.o

install : ccalc
	install --strip ccalc /usr/local/bin/=

uninstall :
	rm -f /usr/local/bin/= core

