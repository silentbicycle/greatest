# Warning flags for C and C++:
COMMON_FLAGS += -Wall -Wextra -pedantic -Werror
COMMON_FLAGS += -Wmissing-declarations -g
#COMMON_FLAGS += -Weverything

CFLAGS += ${COMMON_FLAGS}
CPPFLAGS += ${COMMON_FLAGS}

# These warnings are not valid for C++:
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wstrict-prototypes

PROGRAMS_C=	example example_no_suite example_no_runner \
		example_shuffle example_trunc
PROGRAMS_CPP=	example_cpp

# Uncomment to demo c99 parametric testing.
#CFLAGS += -std=c99

# Uncomment to enable setjmp()/longjmp().
#CFLAGS += -DGREATEST_USE_LONGJMP=1

# Uncomment to disable clock() / time.h.
#CFLAGS += -DGREATEST_USE_TIME=0

all: all_c

all_c: ${PROGRAMS_C}
all_cpp: ${PROGRAMS_CPP}

example: example.o example_suite.o
example_no_suite: example_no_suite.o
example_no_runner: example_no_runner.o
example_shuffle: example_shuffle.o
example_trunc: example_trunc.o

*.o: greatest.h Makefile

example_cpp: example_cpp.cpp greatest.h Makefile
	${CXX} -o $@ example_cpp.cpp ${CPPFLAGS} ${LDFLAGS}

%.o: %.c
	${CC} -c -o $@ ${CFLAGS} $<

%.o: %.cpp
	${CXX} -c -o $@ ${CPPFLAGS} $<

%: %.o
	${CC} -o $@ ${LDFLAGS} $^

clean:
	rm -f ${PROGRAMS_C} ${PROGRAMS_CPP} *.o *.core
