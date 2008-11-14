VERSION=1.2.4
BUILDDATE=20081114

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

