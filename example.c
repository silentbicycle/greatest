#include <stdlib.h>
#include <stdio.h>

#include "greatest.h"

/* Define a suite, compiled seperately. */
extern SUITE(other_suite);

/* Just test against the output of random, to show a
 * variety of results. */
TEST example_test_case() {
    ASSERT(1 == 1);

    int r = random() % 10;
    if (r == 1) SKIP();
    ASSERT(r >= 1);
    PASS();
}

TEST expect_equal() {
    int i = 9;
    ASSERT_EQ(10, i);
    PASS();
}

TEST  expect_str_equal() {
    char *foo = "foo";
    ASSERT_STR_EQ("bar", foo);
    PASS();
}

SUITE(suite) {
    int i=0;
    for (i=0; i<200; i++)
        RUN_TEST(example_test_case);
    RUN_TEST(expect_equal);
    RUN_TEST(expect_str_equal);
}

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    RUN_SUITE(suite);
    RUN_SUITE(other_suite);
    GREATEST_MAIN_END();        /* display results */
}
