include ../config.mk

.PHONY : all clean install uninstall dist dist-clean

COUNTRIES=pl

all : 

dist : ralcalc.1
	@for c in ${COUNTRIES}; do \
		$(DOCBOOK2MAN) --encoding utf8 --to-stdout ralcalc-$${c}.1.xml > ralcalc-$${c}.1; \
	done

clean :

install :
	$(INSTALL) -d ${DESTDIR}$(mandir)/man1
	$(INSTALL) ralcalc.1 ${DESTDIR}${mandir}/man1/ralcalc.1
	$(INSTALL) =.1 ${DESTDIR}${mandir}/man1/=.1
	@for c in ${COUNTRIES}; do \
		$(INSTALL) ralcalc-$${c}.1 ${DESTDIR}${mandir}/$${c}/man1/ralcalc.1 ; \
		$(INSTALL) =.1 ${DESTDIR}${mandir}/$${c}/man1/=.1 ; \
	done

uninstall :
	-rm -f ${DESTDIR}${mandir}/man1/ralcalc.1 ${DESTDIR}${mandir}/man1/=.1
	@for c in ${COUNTRIES}; do \
		rm -f ${DESTDIR}${mandir}/$${c}/man1/ralcalc.1 ${DESTDIR}${mandir}/$${c}/man1/=.1 ; \
	done

ralcalc.1 : ralcalc.1.xml
	$(DOCBOOK2MAN) ralcalc.1.xml

dist-clean :
	-rm -f ralcalc.1
	@for c in ${COUNTRIES}; do \
		rm -f ralcalc-$${c}.1 ; \
	done
