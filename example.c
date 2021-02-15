#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "greatest.h"

/* Define a suite, compiled seperately. */
SUITE_EXTERN(other_suite);

/* Declare a local suite. */
SUITE(suite);

enum foo_t { FOO_1, FOO_2, FOO_3 };
static greatest_enum_str_fun foo_str;

/* Just test against random ints, to show a variety of results. */
TEST example_test_case(void) {
    int r = 0;
    ASSERT(1 == 1);

    r = rand() % 10;
    if (r == 1) SKIP();
    ASSERT(r >= 1);
    PASS();
}

TEST expect_equal(void) {
    int i = 9;
    ASSERT_EQ(10, i);
    PASS();
}

TEST expect_not_equal(void) {
    int i = 9;
    ASSERT_NEQ(10, i);
    PASS();
}

TEST expect_str_equal(void) {
    const char *foo1 = "foo1";
    ASSERT_STR_EQ("foo2", foo1);
    PASS();
}

TEST expect_strn_equal(void) {
    const char *foo1 = "foo1";
    ASSERT_STRN_EQ("foo2", foo1, 3);
    PASSm("custom PASSm message");
}

/* A boxed int type, used to show type-specific equality tests. */
typedef struct {
    int i;
} boxed_int;

/* Callback used to check whether two boxed_ints are equal. */
static int boxed_int_equal_cb(const void *exp, const void *got, void *udata) {
    const boxed_int *ei = (const boxed_int *)exp;
    const boxed_int *gi = (const boxed_int *)got;

    /* udata is not used here, but could be used to specify a comparison
     * resolution, a string encoding, or any other state that should be
     * passed along to the equal and print callbacks. */
    (void)udata;
    return (ei->i == gi->i);
}

/* Callback to print a boxed_int, used to produce an
 * "Executed X, got Y" failure message. */
static int boxed_int_printf_cb(const void *t, void *udata) {
    const boxed_int *bi = (const boxed_int *)t;
    (void)udata;
    return printf("{%d}", bi->i);
}

/* The struct that stores the previous two functions' pointers. */
static greatest_type_info boxed_int_type_info = {
    boxed_int_equal_cb,
    boxed_int_printf_cb,
};

TEST expect_boxed_int_equal(void) {
    boxed_int a = {3};
    boxed_int b = {3};
    boxed_int c = {4};
    ASSERT_EQUAL_T(&a, &b, &boxed_int_type_info, NULL);  /* succeeds */
    ASSERT_EQUAL_T(&a, &c, &boxed_int_type_info, NULL);  /* fails */
    PASS();
}

/* The struct that stores the previous two functions' pointers. */
static greatest_type_info boxed_int_type_info_no_print = {
    boxed_int_equal_cb,
    NULL,
};

TEST expect_boxed_int_equal_no_print(void) {
    boxed_int a = {3};
    boxed_int b = {3};
    boxed_int c = {4};
    (void)boxed_int_printf_cb;
    /* succeeds */
    ASSERT_EQUAL_T(&a, &b, &boxed_int_type_info_no_print, NULL);
    /* fails */
    ASSERT_EQUAL_T(&a, &c, &boxed_int_type_info_no_print, NULL);
    PASS();
}

TEST expect_int_equal_printing_hex(void) {
    unsigned int a = 0xba5eba11;
    unsigned int b = 0xf005ba11;
    ASSERT_EQ_FMT(a, b, "0x%08x");
    PASS();
}

TEST expect_floating_point_range(void) {
    ASSERT_IN_RANGEm("in range",    -0.00001, -0.000110, 0.00010);
    ASSERT_IN_RANGEm("in range",     0.00001,  0.000110, 0.00010);
    ASSERT_IN_RANGE(0.00001,  0.000110, 0.00010);
    ASSERT_IN_RANGEm("out of range", 0.00001,  0.000111, 0.00010);
    PASS();
}

/* Flag, used to confirm that teardown hook is being called. */
static int teardown_was_called = 0;

TEST teardown_example_PASS(void) {
    teardown_was_called = 0;
    PASS();
}

TEST teardown_example_FAIL(void) {
    teardown_was_called = 0;
    FAILm("Using FAIL to trigger teardown callback");
}

TEST teardown_example_SKIP(void) {
    teardown_was_called = 0;
    SKIPm("Using SKIP to trigger teardown callback");
}

/* Example of a test case that calls another function which uses ASSERT. */
static enum greatest_test_res less_than_three(int arg) {
    ASSERT(arg <3);
    PASS();
}

TEST example_using_subfunctions(void) {
    CHECK_CALL(less_than_three(1)); /* <3 */
    CHECK_CALL(less_than_three(5)); /* </3 */
    PASS();
}

/* Example of an ANSI C compatible way to do test cases with
 * arguments: they are passed one argument, a pointer which
 * should be cast back to a struct with the other data. */
TEST parametric_example_c89(void *closure) {
    int arg = *(int *) closure;
    ASSERT(arg > 10);
    PASS();
}

/* If using C99, greatest can also do parametric tests without
 * needing to manually manage a closure. */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 19901L
TEST parametric_example_c99(int arg) {
    ASSERT(arg > 10);
    PASS();
}
#endif

#if GREATEST_USE_LONGJMP
static enum greatest_test_res subfunction_with_FAIL_WITH_LONGJMP(int arg) {
    if (arg == 0) {
        FAIL_WITH_LONGJMPm("zero argument (expected failure)");
    }
    PASS();
}

static enum greatest_test_res subfunction_with_ASSERT_OR_LONGJMP(int arg) {
    ASSERT_OR_LONGJMPm("zero argument (expected failure)", arg != 0);
    PASS();
}

TEST fail_via_FAIL_WITH_LONGJMP(void) {
    subfunction_with_FAIL_WITH_LONGJMP(0);
    PASS();
}

TEST fail_via_FAIL_WITH_LONGJMP_if_0(int arg) {
    subfunction_with_FAIL_WITH_LONGJMP(arg);
    PASS();
}

TEST fail_via_ASSERT_OR_LONGJMP(void) {
    subfunction_with_ASSERT_OR_LONGJMP(0);
    PASS();
}
#endif

TEST expect_mem_equal(void) {
    char got[56];
    char exp[sizeof(got)];
    size_t i = 0;
    for (i = 0; i < sizeof(got); i++) {
        exp[i] = (char)i;
        got[i] = (char)i;
    }

    /* Two bytes differ */
    got[23] = 'X';
    got[34] = 'X';

    ASSERT_MEM_EQm("expected matching memory", exp, got, sizeof(got));
    PASS();
}

static const char *foo_str(int v) {
    switch ((enum foo_t)v) {
    case FOO_1: return "FOO_1";
    case FOO_2: return "FOO_2";
    case FOO_3: return "FOO_3";
    }
    return "unknown";
}

static int side_effect = 0;

static enum foo_t foo_2_with_side_effect(void) {
    side_effect++;
    return FOO_2;
}

TEST expect_enum_equal(void) {
    ASSERT_ENUM_EQ(FOO_1, foo_2_with_side_effect(), foo_str);
    PASS();
}

TEST expect_enum_equal_only_evaluates_args_once(void) {
    /* If the failure case for ASSERT_ENUM_EQ evaluates GOT more
     * than once, `side_effect` will be != 1 here. */
    ASSERT_EQ_FMTm("ASSERT_ENUM_EQ should only evaluate arguments once",
        1, side_effect, "%d");
    PASS();
}

static size_t Fibonacci(unsigned char x) {
    if (x < 2) {
        return 1;
    } else {
        return Fibonacci(x - 1) + Fibonacci(x - 2);
    }
}

TEST extra_slow_test(void) {
    unsigned char i;
    printf("\nThis test can be skipped with a negative test filter...\n");
    for (i = 1; i < 40; i++) {
        printf("fib %u -> %lu\n", i, (long unsigned)Fibonacci(i));
    }
    PASS();
}

TEST nested_RUN_TEST(void) {
    printf("This nested RUN_TEST call should not trigger an infinite loop...\n");
    RUN_TEST(nested_RUN_TEST);
    PASS();
}

TEST eq_pass_and_fail(void) {
    const int x = 1, y = 2;
    ASSERT_EQ(x, x);
    ASSERT_EQm("y == y", y, y);
    ASSERT_EQ(x, y);
    PASS();
}

TEST neq_pass_and_fail(void) {
    const int x = 1, y = 2;
    ASSERT_NEQm("x != y", x, y);
    ASSERT_NEQ(x, x);
    PASS();
}

TEST gt_pass_and_fail(void) {
    const int x = 1, y = 2;
    ASSERT_GTm("y > x", y, x);
    ASSERT_GT(x, x);
    PASS();
}

TEST gte_pass_and_fail(void) {
    const int x = 1, y = 2, z = 3;;
    ASSERT_GTE(z, y);
    ASSERT_GTE(y, x);
    ASSERT_GTE(z, x);
    ASSERT_GTEm("y >= y", y, y);
    ASSERT_GTE(y, z);
    PASS();
}

TEST lt_pass_and_fail(void) {
    const int x = 1, y = 2;
    ASSERT_LTm("x < y", x, y);
    ASSERT_LT(x, x);
    PASS();
}

TEST lte_pass_and_fail(void) {
    const int x = 1, y = 2, z = 3;;
    ASSERT_LTE(y, z);
    ASSERT_LTEm("x <= y", x, y);
    ASSERT_LTE(x, x);
    ASSERT_LTE(z, x);
    PASS();
}

static void trace_setup(void *arg) {
    printf("-- in setup callback\n");
    teardown_was_called = 0;
    (void)arg;
}

static void trace_teardown(void *arg) {
    printf("-- in teardown callback\n");
    teardown_was_called = 1;
    (void)arg;
}

/* Primary test suite. */
SUITE(suite) {
    volatile int i = 0;
    int arg = 0;
    printf("\nThis should have some failures:\n");
    for (i=0; i<200; i++) {
        RUN_TEST(example_test_case);
    }
    RUN_TEST(expect_equal);
    printf("\nThis should fail:\n");
    RUN_TEST(expect_str_equal);
    printf("\nThis should pass:\n");
    RUN_TEST(expect_not_equal);
    RUN_TEST(expect_strn_equal);
    printf("\nThis should fail:\n");
    RUN_TEST(expect_boxed_int_equal);
    printf("\nThis should fail:\n");
    RUN_TEST(expect_boxed_int_equal_no_print);

    printf("\nThis should fail, printing the mismatched values in hex.\n");
    RUN_TEST(expect_int_equal_printing_hex);

    printf("\nThis should fail and show floating point values just outside the range.\n");
    RUN_TEST(expect_floating_point_range);

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

    /* clear setup and teardown */
    GREATEST_SET_SETUP_CB(NULL, NULL);
    GREATEST_SET_TEARDOWN_CB(NULL, NULL);

    printf("This should fail, but note the subfunction that failed.\n");
    RUN_TEST(example_using_subfunctions);

    /* Run a test with one void* argument (which can point to a
     * struct with multiple arguments). */
    printf("\nThis should fail:\n");
    arg = 10;
    RUN_TEST1(parametric_example_c89, &arg);
    arg = 11;
    RUN_TEST1(parametric_example_c89, &arg);

    /* Run a test, with arguments. ('p' for "parametric".) */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 19901L
    printf("\nThis should fail:\n");
    RUN_TESTp(parametric_example_c99, 10);
    RUN_TESTp(parametric_example_c99, 11);
#endif

#if GREATEST_USE_LONGJMP
    RUN_TEST(fail_via_FAIL_WITH_LONGJMP);
    RUN_TEST1(fail_via_FAIL_WITH_LONGJMP_if_0, 0);
    RUN_TEST(fail_via_ASSERT_OR_LONGJMP);
#endif

#if GREATEST_USE_LONGJMP &&                                     \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 19901L)
    RUN_TESTp(fail_via_FAIL_WITH_LONGJMP_if_0, 0);
#endif

    if (GREATEST_IS_VERBOSE()) {
        printf("greatest was run with verbosity level: %u\n",
            greatest_get_verbosity());
    }

    printf("\nThis should fail:\n");
    RUN_TEST(expect_mem_equal);

    printf("\nThis should fail:\n");
    RUN_TEST(expect_enum_equal);

    printf("\nThis should NOT fail:\n");
    RUN_TEST(expect_enum_equal_only_evaluates_args_once);

    RUN_TEST(extra_slow_test);
    RUN_TEST(nested_RUN_TEST);

    printf("\nThese next several tests should also fail:\n");
    RUN_TEST(eq_pass_and_fail);
    RUN_TEST(neq_pass_and_fail);
    RUN_TEST(gt_pass_and_fail);
    RUN_TEST(gte_pass_and_fail);
    RUN_TEST(lt_pass_and_fail);
    RUN_TEST(lte_pass_and_fail);
}

TEST standalone_test(void) {
    FAILm("(expected failure)");
}

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */

    /* If tests are run outside of a suite, a default suite is used. */
    RUN_TEST(standalone_test);

    RUN_SUITE(suite);
    RUN_SUITE(other_suite);

    /* Standalone tests can appear before or after other suites. */
    RUN_TEST(standalone_test);

    GREATEST_MAIN_END();        /* display results */
}
