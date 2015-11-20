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
    struct greatest_report_t report;
    (void)argc;
    (void)argv;
    
    /* Initialize greatest, but don't build the CLI test runner code. */
    GREATEST_INIT();

    RUN_TEST(standalone_pass);

    /* Print report, but do not exit. */
    printf("\nStandard report, as printed by greatest:\n");
    GREATEST_PRINT_REPORT();
    
    greatest_get_report(&report);

    printf("\nCustom report:\n");
    printf("pass %u, fail %u, skip %u, assertions %u\n",
        report.passed, report.failed, report.skipped, report.assertions);

    if (report.failed > 0) {
        return 1;
    }

    return 0;
}
