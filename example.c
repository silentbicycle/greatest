#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "greatest.h"

/* Define a suite, compiled seperately. */
extern SUITE(other_suite);

/* Declare a local suite. */
SUITE(suite);

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
    const char *foo = "foo";
    ASSERT_STR_EQ("bar", foo);
    PASS();
}

TEST expect_mem_equal() {
    const char *foo = "foo\0\0\0\0";
    ASSERT_MEM_EQ("foo\0abc", foo, 8);
    PASS();
}

static int teardown_was_called = 0;

TEST teardown_example_PASS() {
    teardown_was_called = 0;
    PASS();
}

TEST teardown_example_FAIL() {
    teardown_was_called = 0;
    FAILm("Using FAIL to trigger teardown callback");
}

TEST teardown_example_SKIP() {
    teardown_was_called = 0;
    SKIPm("Using SKIP to trigger teardown callback");
}

TEST parametric_example_c89(void *closure) {
    int arg = *(int *) closure;
    ASSERT(arg > 10);
    PASS();
}

/* If using C99, greatest can also do parametric tests without
 * needing to manually manage a closure. */
#if __STDC_VERSION__ >= 19901L
TEST parametric_example_c99(int arg) {
    ASSERT(arg > 10);
    PASS();
}
#endif

static void trace_setup() {
    printf("-- in setup callback\n");
    teardown_was_called = 0;
}

static void trace_teardown() {
    printf("-- in teardown callback\n");
    teardown_was_called = 1;
}

SUITE(suite) {
    int i=0;
    printf("\nThis should have some failures:\n");
    for (i=0; i<200; i++) {
        RUN_TEST(example_test_case);
    }
    RUN_TEST(expect_equal);
    printf("\nThis should fail:\n");
    RUN_TEST(expect_str_equal);
    RUN_TEST(expect_mem_equal);    

    /* Set so asserts below won't fail if running in list-only or
     * first-fail modes. (setup() won't be called and clear it.) */
    teardown_was_called = -1;

    /* Add setup/teardown for each test case. */
    GREATEST_SET_SETUP_CB(trace_setup, NULL);
    GREATEST_SET_TEARDOWN_CB(trace_teardown, NULL);

    /* Check that the test-specific teardown hook is called. */
    RUN_TEST(teardown_example_PASS);
    assert(teardown_was_called);

    printf("\nThis should fail:\n");
    RUN_TEST(teardown_example_FAIL);
    assert(teardown_was_called);

    printf("This should be skipped:\n");
    RUN_TEST(teardown_example_SKIP);
    assert(teardown_was_called);

    /* Run a test with one void* argument (which can point to a
     * struct with multiple arguments). */
    printf("\nThis should fail:\n");
    i = 10;
    RUN_TEST1(parametric_example_c89, &i);
    i = 11;
    RUN_TEST1(parametric_example_c89, &i);

    /* Run a test, with arguments. ('p' for "parametric".) */
#if __STDC_VERSION__ >= 19901L
    printf("\nThis should fail:\n");
    RUN_TESTp(parametric_example_c99, 10);
    RUN_TESTp(parametric_example_c99, 11);
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
