CC=gcc
NAME=ccalc
CFLAGS=-ggdb -Wall
LDFLAGS=-nopie

all : ${NAME}

$(NAME) : ccalc.o list.o
	$(CC) $(LDFLAGS) -o = $^

ccalc.o : ccalc.c ccalc.h
	$(CC) ${CFLAGS} -c -o $@ $<

list.o : list.c list.h ccalc.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	-rm -f \= *.o

