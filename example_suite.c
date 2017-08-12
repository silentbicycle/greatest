#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"

/* Declare a local suite. */
SUITE(other_suite);

TEST blah(void) {
    PASS();
}

TEST todo(void) {
    SKIPm("TODO");
}

GREATEST_SUITE(other_suite) {
    RUN_TEST(blah);
    RUN_TEST(todo);
}
