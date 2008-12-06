include ../config.mk

.PHONY : all clean install uninstall dist dist-clean

all : ralcalc.1

ralcalc.1 : dist

clean :

install :
	$(INSTALL) -d ${DESTDIR}$(mandir)/man1
	$(INSTALL) ralcalc.1 ${DESTDIR}${mandir}/man1/ralcalc.1
	$(INSTALL) =.1 ${DESTDIR}${mandir}/man1/=.1

uninstall :
	-rm -f ${DESTDIR}${mandir}/man1/ralcalc.1 ${DESTDIR}${mandir}/man1/=.1

dist :
	$(DOCBOOK2MAN) ralcalc.1.xml

dist-clean :
	-rm -f ralcalc.1
