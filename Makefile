CFLAGS += -g

all: example

example: example.c greatest.h example-suite.o
	${CC} -o $@ example.c example-suite.o ${CFLAGS} ${LDFLAGS}

clean:
	rm -f example *.o *.core
