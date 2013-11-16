CFLAGS += -Wall -Werror -Wextra -pedantic
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wmissing-declarations

# Uncomment to demo c99 parametric testing.
#CFLAGS += -std=c99

all: example

example: example.c greatest.h example-suite.o
	${CC} -o $@ example.c example-suite.o ${CFLAGS} ${LDFLAGS}

clean:
	rm -f example *.o *.core

example-suite.o: example-suite.c
