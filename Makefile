CFLAGS += -Wall -Werror -Wextra -pedantic
CFLAGS += -Wmissing-declarations
# These two are not valid for C++:
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wstrict-prototypes

PROGRAMS=	example example_no_suite example_no_runner

# Uncomment to demo c99 parametric testing.
#CFLAGS += -std=c99

# Uncomment to disable setjmp()/longjmp().
#CFLAGS += -DGREATEST_USE_LONGJMP=0

# Uncomment to disable clock() / time.h.
#CFLAGS += -DGREATEST_USE_TIME=0

all: ${PROGRAMS}

example: example.c greatest.h example-suite.o
	${CC} -o $@ example.c example-suite.o ${CFLAGS} ${LDFLAGS}

*.o: Makefile
*.o: greatest.h

clean:
	rm -f ${PROGRAMS} *.o *.core

example-suite.o: example-suite.c
