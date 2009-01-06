VERSION=1.3.0
BUILDDATE=20081218

# Add "-DDEBUG" to CFLAGS for verbose debug output
CFLAGS=-fno-common -Wall -Wdeclaration-after-statement -Wextra -Wformat=2 -Winit-self -Winline -Wpacked -Wp,-D_FORTIFY_SOURCE=2 -Wpointer-arith -Wlarger-than-65500 -Wmissing-declarations -Wmissing-format-attribute -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wredundant-decls -Wsign-compare -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch-enum -Wundef -Wunreachable-code -Wwrite-strings -ggdb -Wall -O2
#CFLAGS=-ggdb -Wall -O2
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

