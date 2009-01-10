VERSION=1.3.1.1
BUILDDATE=20090110

# Add "-DDEBUG" to CFLAGS for verbose debug output
CFLAGS=-ggdb -Wall -O2
LDFLAGS=-nopie -lm

CC=gcc
INSTALL=install
XGETTEXT=xgettext
MSGMERGE=msgmerge
MSGFMT=msgfmt
DOCBOOK2MAN=docbook2man.pl

prefix=/usr/local
mandir=${prefix}/share/man
localedir=${prefix}/share/locale

