include config.mk

DIRS=man po src

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
	mkdir -p ralcalc-${VERSION}
	cp -r man po src LICENCE.txt Makefile changes.txt readme.txt ralcalc-${VERSION}/
	tar -jcf ralcalc-${VERSION}.tar.bz2 ralcalc-${VERSION}/

dist-clean : clean
	-rm -rf ralcalc-${VERSION}*

sign : dist
	gpg --detach-sign -a ralcalc-${VERSION}.tar.bz2

copy : sign
	scp ralcalc-${VERSION}.tar.bz2 ralcalc-${VERSION}.tar.bz2.asc atchoo:atchoo.org/tools/ralcalc/files/
