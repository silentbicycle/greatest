#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "greatest.h"

TEST standalone_pass(void) {
    PASS();
}

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    
    /* Initialize greatest, but don't build the CLI test runner code. */
    GREATEST_INIT();

    RUN_TEST(standalone_pass);

    /* Report, but do not exit. */
    GREATEST_REPORT();
    
    return 0;
}
