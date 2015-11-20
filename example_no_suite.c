#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "greatest.h"

TEST standalone_fail(void) {
    FAILm("(expected failure)");
}

TEST standalone_pass(void) {
    PASS();
}

TEST standalone_skip(void) {
    SKIPm("skipped");
}

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    /* If tests are run outside of a suite, a default suite is used. */
    RUN_TEST(standalone_fail);
    RUN_TEST(standalone_pass);
    RUN_TEST(standalone_skip);

    GREATEST_MAIN_END();        /* display results */
}
