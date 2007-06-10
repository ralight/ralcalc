CC=gcc
CFLAGS=-ggdb -Wall -O2
LDFLAGS=-nopie -lm
VERSION=20070610

PREFIX=/usr/local
MANDIR=/usr/local/man

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
	install --strip ralcalc ${PREFIX}/bin/ralcalc
	ln -sf ${PREFIX}/bin/ralcalc ${PREFIX}/bin/=
	install ralcalc.1 ${MANDIR}/man1/ralcalc.1
	install =.1 ${MANDIR}/man1/=.1

uninstall :
	rm -f ${PREFIX}/bin/= ${PREFIX}/bin/ralcalc
	rm -f ${MANDIR}/ralcalc.1 ${MANDIR}/man1/=.1


dist :
	mkdir -p ralcalc-${VERSION}
	cp LICENCE.txt Makefile calculation.c calculation.h changes.txt datatypes.h output.c output.h ralcalc.c ralcalc.1 readme.txt tokens.c tokens.h ralcalc-${VERSION}/
	tar -jcf ralcalc-${VERSION}.tar.bz2 ralcalc-${VERSION}/

sign : dist
	gpg --detach-sign -a ralcalc-${VERSION}.tar.bz2
