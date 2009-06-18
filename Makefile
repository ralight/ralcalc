include config.mk

DIRS=man po src
DISTDIRS=man

.PHONY : all ralcalc clean install uninstall dist distclean sign copy

all : ralcalc

ralcalc :
	for d in ${DIRS}; do $(MAKE) -C $${d}; done

clean :
	for d in ${DIRS}; do $(MAKE) -C $${d} clean; done

install : ralcalc
	@for d in ${DIRS}; do $(MAKE) -C $${d} install; done

uninstall :
	@for d in ${DIRS}; do $(MAKE) -C $${d} uninstall; done

dist : clean
	@for d in ${DISTDIRS}; do $(MAKE) -C $${d} dist; done
	
	mkdir -p dist/ralcalc-${VERSION}
	cp -r man po src COPYING Makefile ChangeLog config.mk readme.txt dist/ralcalc-${VERSION}/
	cd dist; tar -zcf ralcalc-${VERSION}.tar.gz ralcalc-${VERSION}/

distclean : clean
	@for d in ${DISTDIRS}; do $(MAKE) -C $${d} distclean; done
	
	-rm -rf dist/

sign : dist
	gpg --detach-sign -a ralcalc-${VERSION}.tar.gz

copy : sign
	man2html man/ralcalc.1 > dist/ralcalc.html
	@for c in ${MANCOUNTRIES}; do man2html man/ralcalc-$${c}.1 > dist/ralcalc-$${c}.html; done
	cd dist; scp ralcalc-${VERSION}.tar.gz ralcalc-${VERSION}.tar.gz.asc atchoo:atchoo.org/tools/ralcalc/files/
	cd dist; scp ralcalc.html ralcalc-da.html ralcalc-es.html ralcalc-fr.html ralcalc-pl.html ralcalc-pt_BR.html atchoo:atchoo.org/tools/ralcalc/

pottar : distclean
	mkdir -p dist/ralcalc-pot/
	for a in $$(ls po/*.po); do cp $${a} dist/ralcalc-pot/ralcalc-$$(basename $${a}); done
	cp po/ralcalc.pot dist/ralcalc-pot/
	cd dist; tar -zcf ralcalc-pot.tar.gz ralcalc-pot
	
	mkdir -p dist/ralcalc-man-pot/
	/usr/bin/xml2po -o dist/ralcalc-man-pot/ralcalc-man-1.pot man/ralcalc.1.xml
	for c in ${MANCOUNTRIES}; do \
		/usr/bin/xml2po -o dist/ralcalc-man-pot/ralcalc-man-1-$${c}.po man/ralcalc-$${c}.1.xml; \
	done
	
	cd dist; tar -zcf ralcalc-man-pot.tar.gz ralcalc-man-pot

