#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <sys/time.h>
#include <err.h>

#include "greatest.h"

#define TEST_COUNT 1000

static char test_has_run[(TEST_COUNT / 8) + 1];

static int check_run(unsigned int id) {
    size_t offset = id / 8;
    char bit = 1U << (id & 0x07);
    return 0 != (test_has_run[offset] & bit);
}

static void set_run(unsigned int id) {
    size_t offset = id / 8;
    char bit = 1U << (id & 0x07);
    test_has_run[offset] |= bit;
}

static void reset_run(void) {
    memset(test_has_run, 0x00, sizeof(test_has_run));
}

static int print_flag = 0;

TEST print_and_pass(unsigned int id) {
    if (print_flag) {
        printf("running test %u\n", id);
    }
    if (check_run(id)) {
        fprintf(stderr, "Error: Shuffling made test run multiple times!\n");
        assert(0);
    } else {
        set_run(id);
    }
    PASS();
}

static unsigned int seed_of_time(void) {
    static unsigned int counter = 1;
    struct timeval tv;
    if (0 != gettimeofday(&tv, NULL)) {
        err(1, "gettimeofday");
    }
    return ~(tv.tv_sec ^ tv.tv_usec) * counter++;
}

SUITE(suite) {
    const unsigned int limit = TEST_COUNT;
    unsigned int count;
    const unsigned int small_test_count = 10;
    unsigned int i = 0;

    /* Check that all are run exactly once, for a small number of tests */
    print_flag = 1;
    for (count = 0; count < small_test_count; count++) {
        unsigned int seed = seed_of_time();
        fprintf(stderr, "count %u, seed %u\n", count, seed);
        SHUFFLE_TESTS(seed, {
            if (count > 0) { RUN_TEST1(print_and_pass, 0); }
            if (count > 1) { RUN_TEST1(print_and_pass, 1); }
            if (count > 2) { RUN_TEST1(print_and_pass, 2); }
            if (count > 3) { RUN_TEST1(print_and_pass, 3); }
            if (count > 4) { RUN_TEST1(print_and_pass, 4); }
            if (count > 5) { RUN_TEST1(print_and_pass, 5); }
            if (count > 6) { RUN_TEST1(print_and_pass, 6); }
            if (count > 7) { RUN_TEST1(print_and_pass, 7); }
            if (count > 8) { RUN_TEST1(print_and_pass, 8); }
        });

        for (i = 0; i < count; i++) {
            if (!check_run(i)) {
                fprintf(stderr, "Error: Test %u got lost in the shuffle!\n", i);
                assert(0);
            }
        }
        reset_run();
    }
    print_flag = 0;

    /* Check that all are run exactly once, for a larger amount of tests */
    SHUFFLE_TESTS(seed_of_time(), {
        for (i = 0; i < limit; i++) {
            RUN_TEST1(print_and_pass, i);
        }
    });

    for (i = 0; i < limit; i++) {
        if (!check_run(i)) {
            fprintf(stderr, "Error: Test %u got lost in the shuffle!\n", i);
            assert(0);
        }
    }
}

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */

    RUN_SUITE(suite);

    GREATEST_MAIN_END();        /* display results */
}
