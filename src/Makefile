include ../config.mk

ralcalc_SOURCES= \
	calculation.c \
	output.c \
	ralcalc.c \
	tokens.c
ralcalc_OBJS=$(ralcalc_SOURCES:.c=.o)

.PHONY : all clean install uninstall

all : ralcalc ralcalc_test

ralcalc : $(ralcalc_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) -DVERSION=\"${VERSION}\" -DLOCALEDIR=\"${localedir}\" ${CFLAGS} -c $^

clean :
	-rm -f ralcalc *.o ralcalc_test *.gcda *.gcno

install : ralcalc
	$(INSTALL) -d ${DESTDIR}${prefix}/bin/
	$(INSTALL) -s ralcalc ${DESTDIR}${prefix}/bin/ralcalc
	ln -sf ${prefix}/bin/ralcalc ${DESTDIR}${prefix}/bin/=

uninstall :
	-rm -f ${DESTDIR}${prefix}/bin/= ${DESTDIR}${prefix}/bin/ralcalc

# Testing
#
ralcalc_test : test.o output.o
	$(CC) -o $@ $^ $(LDFLAGS) -lcunit

test : ralcalc_test
	./ralcalc_test
