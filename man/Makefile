include ../config.mk

.PHONY : all clean install uninstall dist distclean

all : 

dist : ralcalc.1
	for c in ${MANCOUNTRIES}; do \
		$(XSLTPROC) ralcalc-$${c}.1.xml; \
	done

clean :
	-rm ralcalc*.1

install :
	$(INSTALL) -d ${DESTDIR}$(mandir)/man1
	$(INSTALL) -m 644 ralcalc.1 ${DESTDIR}${mandir}/man1/ralcalc.1
	$(INSTALL) -m 644 =.1 ${DESTDIR}${mandir}/man1/=.1
	@for c in ${MANCOUNTRIES}; do \
		d=$$(echo $${c} | sed -e 's/_/-/g') ; \
		echo $${d} ; \
		$(INSTALL) -m 644 -D ralcalc.$${d}.1 ${DESTDIR}${mandir}/$${c}/man1/ralcalc.1 ; \
		$(INSTALL) -m 644 -D =.1 ${DESTDIR}${mandir}/$${c}/man1/=.1 ; \
	done

uninstall :
	-rm -f ${DESTDIR}${mandir}/man1/ralcalc.1 ${DESTDIR}${mandir}/man1/=.1
	@for c in ${MANCOUNTRIES}; do \
		rm -f ${DESTDIR}${mandir}/$${c}/man1/ralcalc.1 ${DESTDIR}${mandir}/$${c}/man1/=.1 ; \
	done

ralcalc.1 : ralcalc.1.xml
	$(XSLTPROC) $^

distclean :
	-rm -f ralcalc.1
	@for c in ${MANCOUNTRIES}; do \
		rm -f ralcalc-$${c}.1 ; \
	done

potgen :
	xml2po -o po/ralcalc.1.pot ralcalc.1.xml
	@for c in ${MANCOUNTRIES}; do \
		xml2po -o po/$${c}.po ralcalc-$${c}.1.xml; \
	done

# To merge new translations do:
# /usr/bin/xml2po -p de.po chapter1.xml > chapter1.de.xml
