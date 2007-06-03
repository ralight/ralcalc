CC=gcc
CFLAGS=-ggdb -Wall -O2
LDFLAGS=-nopie -lm
VERSION=20070603

all : ralcalc

ralcalc : calculation.o output.o ralcalc.o tokens.o
	$(CC) $(LDFLAGS) -o $@ $^

calculation.o : calculation.c calculation.h datatypes.h
	$(CC) ${CFLAGS} -c -o $@ $<

output.o : output.c output.h datatypes.h
	$(CC) $(CFLAGS) -c -o $@ $<

ralcalc.o : ralcalc.c datatypes.h
	$(CC) -DVERSION=\"${VERSION}\" ${CFLAGS} -c -o $@ $<

tokens.o : tokens.c tokens.h datatypes.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	-rm -f ralcalc *.o

install : ralcalc
	install --strip ralcalc /usr/local/bin/ralcalc
	ln -sf /usr/local/bin/ralcalc /usr/local/bin/=

uninstall :
	rm -f /usr/local/bin/= /usr/local/bin/ralcalc core

