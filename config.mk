VERSION=1.6.0

MANCOUNTRIES=da en_GB es eu fr it nl pl pt pt_BR tr vi

# Add "-DDEBUG" to CFLAGS for verbose debug output
#COVERAGE=-coverage
CFLAGS=-ggdb -Wall -O2 ${COVERAGE}
LDFLAGS=-lm ${COVERAGE}

CC=gcc
INSTALL=install
XGETTEXT=xgettext
MSGMERGE=msgmerge
MSGFMT=msgfmt

prefix=/usr/local
mandir=${prefix}/share/man
localedir=${prefix}/share/locale

# Man page generation requires xsltproc and docbook-xsl
XSLTPROC=xsltproc
# For html generation
DB_HTML_XSL=man/html.xsl
