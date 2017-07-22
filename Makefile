# Warning flags for C and C++:
COMMON_FLAGS += -Wall -Wextra -pedantic -Werror
COMMON_FLAGS += -Wmissing-declarations
#COMMON_FLAGS += -Weverything

CFLAGS += ${COMMON_FLAGS}
CPPFLAGS += ${COMMON_FLAGS}

# These warnings are not valid for C++:
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wstrict-prototypes

PROGRAMS_C=	example example_no_suite example_no_runner example_random
PROGRAMS_CPP=	example_cpp

# Uncomment to demo c99 parametric testing.
#CFLAGS += -std=c99
#CFLAGS += -std=c89

# Uncomment to disable setjmp()/longjmp().
#CFLAGS += -DGREATEST_USE_LONGJMP=0

# Uncomment to disable clock() / time.h.
#CFLAGS += -DGREATEST_USE_TIME=0

all: all_c

all_c: ${PROGRAMS_C}
all_cpp: ${PROGRAMS_CPP}

example: example.c greatest.h example-suite.o
	${CC} -o $@ example.c example-suite.o ${CFLAGS} ${LDFLAGS}

example_cpp: example_cpp.cpp
	${CXX} -o $@ example_cpp.cpp ${CPPFLAGS} ${LDFLAGS}

*.o: Makefile
*.o: greatest.h

${PROGRAMS_C}: greatest.h
${PROGRAMS_CPP}: greatest.h

clean:
	rm -f ${PROGRAMS_C} ${PROGRAMS_CPP} *.o *.core

example-suite.o: example-suite.c
