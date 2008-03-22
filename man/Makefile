include ../config.mk

.PHONY : all clean install uninstall

all : 

clean :

install :
	$(INSTALL) -d ${DESTDIR}$(mandir)/man1
	$(INSTALL) ralcalc.1 ${DESTDIR}${mandir}/man1/ralcalc.1
	$(INSTALL) =.1 ${DESTDIR}${mandir}/man1/=.1

uninstall :
	-rm -f ${DESTDIR}${mandir}/man1/ralcalc.1 ${DESTDIR}${mandir}/man1/=.1
