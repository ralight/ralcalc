VERSION=1.2.2
BUILDDATE=20080915

# Add "-DDEBUG" to CFLAGS for verbose debug output
CFLAGS=-ggdb -Wall -O2
LDFLAGS=-nopie -lm

CC=gcc
INSTALL=install
XGETTEXT=xgettext
MSGMERGE=msgmerge
MSGFMT=msgfmt

prefix=/usr/local
mandir=${prefix}/man
localedir=${prefix}/share/locale

