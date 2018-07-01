include ../config.mk

ralcalc_SOURCES= \
	calculation.c \
	output.c \
	ralcalc.c \
	tokens.c
ralcalc_OBJS=$(ralcalc_SOURCES:.c=.o)

.PHONY : all clean install uninstall

all : ralcalc

ralcalc : $(ralcalc_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) -DVERSION=\"${VERSION}\" -DLOCALEDIR=\"${localedir}\" ${CFLAGS} -c $^

clean :
	-rm -f ralcalc *.o

install : ralcalc
	$(INSTALL) -d ${DESTDIR}${prefix}/bin/
	$(INSTALL) -s ralcalc ${DESTDIR}${prefix}/bin/ralcalc
	ln -sf ${prefix}/bin/ralcalc ${DESTDIR}${prefix}/bin/=

uninstall :
	-rm -f ${DESTDIR}${prefix}/bin/= ${DESTDIR}${prefix}/bin/ralcalc
