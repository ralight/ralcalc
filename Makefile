COMPILE=gcc
NAME=ccalc
CFLAGS=-ggdb -Wall
LDFLAGS=-nopie

all : ${NAME}

${NAME} : ${NAME}.c
	$(COMPILE) ${CFLAGS} ${LDFLAGS} -o = $^
	touch ${NAME}

clean :
	-rm -f \= *.o

