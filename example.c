#include <stdlib.h>
#include <stdio.h>

#include "greatest.h"

/* Define a suite, compiled seperately. */
extern SUITE(other_suite);

/* Just test against the output of random, to show a
 * variety of results. */
TEST example_test_case() {
    int r = 0;
    ASSERT(1 == 1);

    r = random() % 10;
    if (r == 1) SKIP();
    ASSERT(r >= 1);
    PASS();
}

TEST expect_equal() {
    int i = 9;
    ASSERT_EQ(10, i);
    PASS();
}

TEST expect_str_equal() {
    char *foo = "foo";
    ASSERT_STR_EQ("bar", foo);
    PASS();
}

static int teardown_was_called = 0;

static void teardown_cb(void *udata) {
    int *flag = (int *) udata;
    (*flag) = 1;
}

TEST teardown_example_PASS() {
    teardown_was_called = 0;
    GREATEST_SET_TEARDOWN_CB(teardown_cb, &teardown_was_called);
    PASS();
}

TEST teardown_example_FAIL() {
    teardown_was_called = 0;
    GREATEST_SET_TEARDOWN_CB(teardown_cb, &teardown_was_called);
    FAILm("Failing to trigger teardown callback");
}

TEST teardown_example_SKIP() {
    teardown_was_called = 0;
    GREATEST_SET_TEARDOWN_CB(teardown_cb, &teardown_was_called);
    SKIPm("Skipping to trigger teardown callback");
}

TEST check_if_teardown_was_called() {
    ASSERT_EQ(1, teardown_was_called);
    PASSm("teardown_was_called");
}

/* If using C99, greatest can also do parametric tests. */
#if __STDC_VERSION__ >= 19901L
TEST parametric_example(int arg) {
    ASSERT(arg > 10);
    PASS();
}
#endif

static void trace_setup() {
    printf("-- in suite setup callback\n");
}

static void trace_teardown() {
    printf("-- in suite teardown callback\n");
}

SUITE(suite) {
    int i=0;
    for (i=0; i<200; i++)
        RUN_TEST(example_test_case);
    RUN_TEST(expect_equal);
    RUN_TEST(expect_str_equal);

    /* Add setup/teardown for each test case. */
    GREATEST_SET_SUITE_SETUP_CB(trace_setup, NULL);
    GREATEST_SET_SUITE_TEARDOWN_CB(trace_teardown, NULL);

    /* Check that the test-specific teardown hook is called. */
    RUN_TEST(teardown_example_PASS);
    RUN_TEST(check_if_teardown_was_called);

    RUN_TEST(teardown_example_FAIL);
    RUN_TEST(check_if_teardown_was_called);

    RUN_TEST(teardown_example_SKIP);
    RUN_TEST(check_if_teardown_was_called);

    /* Run a test, with arguments. ('p' for "parametric".) */
#if __STDC_VERSION__ >= 19901L
    RUN_TESTp(parametric_example, 10);
    RUN_TESTp(parametric_example, 11);
#endif
}

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    RUN_SUITE(suite);
    RUN_SUITE(other_suite);
    GREATEST_MAIN_END();        /* display results */
}
