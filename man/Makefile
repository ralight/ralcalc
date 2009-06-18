include ../config.mk

.PHONY : all clean install uninstall dist distclean

all : 

dist : ralcalc.1
	@for c in ${MANCOUNTRIES}; do \
		$(DOCBOOK2MAN) --encoding utf8 --to-stdout ralcalc-$${c}.1.xml > ralcalc-$${c}.1; \
	done

clean :
	-rm *.1

install :
	$(INSTALL) -d ${DESTDIR}$(mandir)/man1
	$(INSTALL) -m 644 ralcalc.1 ${DESTDIR}${mandir}/man1/ralcalc.1
	$(INSTALL) -m 644 =.1 ${DESTDIR}${mandir}/man1/=.1
	@for c in ${MANCOUNTRIES}; do \
		$(INSTALL) -m 644 -D ralcalc-$${c}.1 ${DESTDIR}${mandir}/$${c}/man1/ralcalc.1 ; \
		$(INSTALL) -m 644 -D =.1 ${DESTDIR}${mandir}/$${c}/man1/=.1 ; \
	done

uninstall :
	-rm -f ${DESTDIR}${mandir}/man1/ralcalc.1 ${DESTDIR}${mandir}/man1/=.1
	@for c in ${MANCOUNTRIES}; do \
		rm -f ${DESTDIR}${mandir}/$${c}/man1/ralcalc.1 ${DESTDIR}${mandir}/$${c}/man1/=.1 ; \
	done

ralcalc.1 : ralcalc.1.xml
	$(DOCBOOK2MAN) ralcalc.1.xml

distclean :
	-rm -f ralcalc.1
	@for c in ${MANCOUNTRIES}; do \
		rm -f ralcalc-$${c}.1 ; \
	done
