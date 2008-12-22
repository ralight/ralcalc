include config.mk

DIRS=man po src
DISTDIRS=man

.PHONY : all ralcalc clean install uninstall dist dist-clean sign copy

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
	
	mkdir -p ralcalc-${VERSION}
	cp -r man po src COPYING Makefile changes.txt config.mk readme.txt ralcalc-${VERSION}/
	tar -jcf ralcalc-${VERSION}.tar.bz2 ralcalc-${VERSION}/

dist-clean : clean
	@for d in ${DISTDIRS}; do $(MAKE) -C $${d} dist-clean; done
	
	-rm -rf ralcalc-${VERSION}*

sign : dist
	gpg --detach-sign -a ralcalc-${VERSION}.tar.bz2

copy : sign
	man2html man/ralcalc.1 > ralcalc.html
	man2html man/ralcalc-pl.1 > ralcalc-pl.html
	scp ralcalc-${VERSION}.tar.bz2 ralcalc-${VERSION}.tar.bz2.asc atchoo:atchoo.org/tools/ralcalc/files/
	scp ralcalc.html ralcalc-pl.html atchoo:atchoo.org/tools/ralcalc/

pottar : dist-clean
	mkdir -p ralcalc-pot/
	for a in $(ls po/*.po); do cp ${a} ralcalc-pot/ralcalc-${a}; done
	cp po/ralcalc.pot ralcalc-pot/
	tar -zcf ralcalc-pot.tar.gz ralcalc-pot
	
	mkdir -p ralcalc-man-pot/
	/usr/bin/xml2po -o ralcalc-man-pot/ralcalc-man-1.pot man/ralcalc.1.xml
	tar -zcf ralcalc-man-pot.tar.gz ralcalc-man-pot
