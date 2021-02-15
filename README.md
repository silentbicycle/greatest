# greatest

A testing system for C, contained in 1 header file.


## Key Features and Project Goals

- **Small, Portable, Lightweight**

    greatest doesn't depend on anything beyond ANSI C89, and the test
    scaffolding should build without warnings when compiled with
    `-Wall -Wextra -pedantic`. It is under 1,000 LOC (SLOCCount),
    and does no dynamic allocation.

- **Permissive License**

    greatest is released under the [ISC License][ISC]. You can use it
    freely, even for commercial purposes.

- **Easy To Set Up**

    To use, just `#include "greatest.h"` in your project. There is
    very little boilerplate. Most features are optional.

- **Un-Opinionated**

    When a command-line test runner is useful, greatest can provide one,
    but it can also run as part of other programs. It doesn't depend on
    a particular build system or other tooling, and should accommodate a
    variety of testing approaches. It actively avoids imposing
    architectural choices on code under test. While greatest was
    designed with C in mind, it attempts to be usable from C++.

- **Modular**

    Tests can be run individually, or grouped into suites. Suites can
    share common setup, and can be in distinct compilation units.

- **Low Friction**

    Specific tests or suites can be run by name, for focused and rapid
    iteration during development. greatest adds very little startup
    latency.


There are some compile-time options, and slightly nicer syntax for
parametric testing (running tests with arguments) if compiled
with a C99 or later language standard.

I wrote a
[blog post](http://spin.atomicobject.com/2013/07/31/greatest-c-testing-embedded/)
with more information. While it's several years old, it's still accurate
about the main functionality.

[theft][], a related project, adds [property-based testing][pbt].

[1]: http://spin.atomicobject.com/2013/07/31/greatest-c-testing-embedded/
[theft]: https://github.com/silentbicycle/theft
[pbt]: https://spin.atomicobject.com/2014/09/17/property-based-testing-c/
[ISC]: https://opensource.org/licenses/ISC

## Basic Usage

```c
#include "greatest.h"

/* A test runs various assertions, then calls PASS(), FAIL(), or SKIP(). */
TEST x_should_equal_1(void) {
    int x = 1;
    /* Compare, with an automatic "1 != x" failure message */
    ASSERT_EQ(1, x);

    /* Compare, with a custom failure message */
    ASSERT_EQm("Yikes, x doesn't equal 1", 1, x);

    /* Compare, and if they differ, print both values,
     * formatted like `printf("Expected: %d\nGot: %d\n", 1, x);` */
    ASSERT_EQ_FMT(1, x, "%d");
    PASS();
}

/* Suites can group multiple tests with common setup. */
SUITE(the_suite) {
    RUN_TEST(x_should_equal_1);
}

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    /* Individual tests can be run directly in main, outside of suites. */
    /* RUN_TEST(x_should_equal_1); */

    /* Tests can also be gathered into test suites. */
    RUN_SUITE(the_suite);

    GREATEST_MAIN_END();        /* display results */
}
```

Output:

```sh
$ make simple && ./simple
cc -g -Wall -Werror -pedantic    simple.c   -o simple

* Suite the_suite:
.
1 test - 1 passed, 0 failed, 0 skipped (5 ticks, 0.000 sec)

Total: 1 test (47 ticks, 0.000 sec), 3 assertions
Pass: 1, fail: 0, skip: 0.
```

Tests are run with `RUN_TEST(test_name)`, which can be called directly
from the test runner's `main` function or grouped into suites (which are
run with `RUN_SUITE(suite_name)`). (Calls to `RUN_TEST` from inside
another test are ignored.)

Test cases can be run with arguments: `RUN_TEST1(test_name, arg)` passes
a single argument, and if C99 features are supported, then
`RUN_TESTp(test_name, ...)` uses `__VA_ARGS__` to run a test case with
one or mare arguments. `greatest_set_test_suffix` sets a name suffix, so
output from the test runner can include info about arguments.

Test cases should call assertions and then end with `PASS()`, `SKIP()`,
`FAIL()`, or their custom message variants (e.g. `SKIPm("TODO");`).
If there are any test failures, the test runner will return 1,
otherwise it will return 0. (Skips do not cause the test runner to
report failure.)

`PASS()`, `SKIP()`, `FAIL()`, and their custom message variants are
macros that updating internal bookkeeping and then returning and enum
value, such as `GREATEST_TEST_RES_FAIL`. They all `return` from the
current test case function.

`PASS()`/`PASSm("msg")` prints as a dot when verbosity is zero, or
the test name and custom message (if any) with verbosity >= 1.

`FAIL()`/`FAILm("msg")` always prints "FAIL test_name: msg file:line".

`SKIP()`/`SKIPm("msg")` prints as an 's' when verbosity is zero, or the
test name and custom message (if any) with verbosity >= 1. Because skips
are not treated as a failure by the test runner, they can be used to
skip test cases that aren't relevant in a particular build or
environment, a way to temporarily disable broken tests, or as a sort of
todo list for tests and functionality under active development.

Tests and suites are just functions, so normal C scoping rules apply.
For example, a test or suite named `main` will have a name collision.

(For more examples, look at `example.c` and `example_suite.c`.)


## Filtering By Name

greatest runs all tests by default, but can be configured to only run
suites or tests whose names contain a filter string, and/or exclude
tests whose name contains a filter string. When test name filtering and
exclusion are used together, exclusion takes precedence.

    void greatest_set_suite_filter(const char *name);
    void greatest_set_test_filter(const char *name);
    void greatest_set_test_exclude(const char *name);

These correspond to the following command line test runner options:

    `-s SUITE`:   Only run suites whose names contain the string "SUITE"
    `-t TEST`:    Only run tests whose names contain the string "TEST"
    `-x EXCLUDE`: Exclude tests whose names contain the string "EXCLUDE"

For example, to run any tests with "tree" in the name, in suites with
"pars" in the name (such as "parser"), but exclude any tests whose names
also contain "slow":

    ./test_project -s pars -t tree -x slow

The string matching includes optional test name suffixes.

The `greatest_set_exact_name_match()` function and corresponding `-e`
command line runner flag can be used to only run tests and/or suites
whose names exactly match the name filter(s). Note: exact-match suite
filtering by name will not skip tests that are run outside of any suite.


## Available Assertions

Assertions fail the current test unless some condition holds. All
assertions have a custom message variant (with an `m` suffix), which
takes a  message string as its first argument. For example, the
assertion `ASSERT_EQ(apple, orange);` could instead be used like
`ASSERT_EQm("these should match", apple, orange)`. Non-message
assertions create a default message.


### `ASSERT(COND)`

Assert that `COND` evaluates to a true (non-zero) value.


### `ASSERT_FALSE(COND)`

Assert that `COND` evaluates to a false (zero) value.


### `ASSERT_EQ(EXPECTED, ACTUAL)`

Assert that `EXPECTED == ACTUAL`. To print the values if they
differ, use `ASSERT_EQ_FMT`. To compare with custom equality test
and print functions, use `ASSERT_EQUAL_T` instead.


### `ASSERT_NEQ(EXPECTED, ACTUAL)`

Assert that `EXPECTED != ACTUAL`.


### `ASSERT_GT(EXPECTED, ACTUAL)`

Assert that `EXPECTED > ACTUAL`.


### `ASSERT_GTE(EXPECTED, ACTUAL)`

Assert that `EXPECTED >= ACTUAL`.


### `ASSERT_LT(EXPECTED, ACTUAL)`

Assert that `EXPECTED < ACTUAL`.


### `ASSERT_LTE(EXPECTED, ACTUAL)`

Assert that `EXPECTED <= ACTUAL`.


### `ASSERT_EQ_FMT(EXPECTED, ACTUAL, FORMAT)`

Assert that `EXPECTED == ACTUAL`. If they are not equal, print their
values using `FORMAT` as the `printf` format string.

For example: `ASSERT_EQ_FMT(123, result, "%d");` will call `printf`
like `printf("Expected: %d\nGot: %d\n", 123, result);` if its
`EXPECTED` and `ACTUAL` arguments don't match.

Note: `EXPECTED` and `ACTUAL` will be evaluated more than once on
failure, so they should not be a function call with side effects.
(Since their type is not known by the macro, they cannot be
captured in a local variable. `typeof` is a GCC extension.)


### `ASSERT_IN_RANGE(EXPECTED, ACTUAL, TOLERANCE)`

Assert that `ACTUAL` is within `EXPECTED` +/- `TOLERANCE`, once the
values have been converted to a configurable floating point type
(`GREATEST_FLOAT`).

greatest does not depent on floating point math.
It is only used within `ASSERT_IN_RANGE`'s macro expansion.


### `ASSERT_STR_EQ(EXPECTED, ACTUAL)`

Assert that the strings are equal
(i.e., `strcmp(EXPECTED, ACTUAL) == 0`).


### `ASSERT_STRN_EQ(EXPECTED, ACTUAL, SIZE)`

Assert that the first `SIZE` bytes of the strings are equal
(i.e., `strncmp(EXPECTED, ACTUAL, SIZE) == 0`).


### `ASSERT_MEM_EQ(EXPECTED, ACTUAL, SIZE)`

Assert that the first `SIZE` bytes of memory pointed to by `EXPECTED`
and `ACTUAL` are equal. If their memory differs, print a hexdump and
highlight the lines and individual bytes which do not match.


### `ASSERT_ENUM_EQ(EXPECTED, ACTUAL, ENUM_STR_FUN)`

Assert that the enum value `EXPECTED` is equal to `ACTUAL`. If not,
convert each enum value to a string using `ENUM_STR_FUN` before printing
them.

`ENUM_STR_FUN` should have a type like:

    const char *FUN(int x);


### `ASSERT_EQUAL_T(EXPECTED, ACTUAL, TYPE_INFO, UDATA)`

Assert that `EXPECTED` and `ACTUAL` are equal, using the
`greatest_equal_cb` function pointed to by `TYPE_INFO->equal` to compare
them. The assertion's `UDATA` argument can be used to pass in arbitrary
user data (or `NULL`) to the callbacks. If the values are not equal and
the `TYPE_INFO->print` function is defined, it will be used to print an
"Expected: X, Got: Y" message.


### `ASSERT_OR_LONGJMP(COND)`

Assert that `COND` evaluates to a true value. If not, then use
`longjmp(3)` to immediately return from the test case and any
intermediate function calls.

If built with `GREATEST_USE_LONGJMP` `#define`d to 0, then all
setjmp/longjmp-related functionality will be compiled out. This also
reduces memory usage by `sizeof jmp_buf`, which may be several hundred
bytes, depending on the platform.


## Random Shuffling

Groups of suites or tests can be run in shuffled order by using
`GREATEST_SHUFFLE_SUITES` and `GREATEST_SHUFFLE_TESTS`, respectively.
This can help find and eliminate accidental coupling between tests.

The shuffling depends on the seed and the test/suite count, so a
consistent seed will only lead to reproducible ordering until the
group's count changes.

Shuffling suites:

    SHUFFLE_SUITES(seed, {
        RUN_SUITE(suite1);
        RUN_SUITE(suite2);
        RUN_SUITE(suite3);
        RUN_SUITE(suite4);
        RUN_SUITE(suite5);
    });

Shuffling tests:

    SHUFFLE_TESTS(seed, {
        RUN_TEST(test_a);
        RUN_TEST1(test_b, 12345);
        RUN_TEST(test_c);
        RUN_TESTp(test_d, "some_argument");
        RUN_TEST(test_e);
    });

Note: Any other code inside the block will be executed several times.
The shuffling macro expands to a loop with (count + 1) iterations -- the
first pass counts, and the following passes only execute the next chosen
suite/test. In particular, avoid running tests directly inside of a
`SHUFFLE_SUITES` block without a suite, because the test will be run
on each iteration.


## Test Name Suffixes

`greatest_set_test_suffix` can be used to set an optional name suffix
for the next test:

    for (i = 0; i < row_count; i++) {
        const struct table_row *row = &table[row_count];
        greatest_set_test_suffix(row->name);
        RUN_TEST1(test_with_arg, row);
    }

This will cause the test name to print with a `_` separator and the
suffix in all pass/fail/skip messages (i.e., `test_with_arg_KEY`). This
is especially useful when running a test several times with different
arguments, in shuffled order. The name suffix is included when using
name-based filtering.

The test name and optional suffix are copied into an internal buffer.
Its size can be configured by `#define`ing the constant
`GREATEST_TESTNAME_BUF_SIZE`. (If not `#define`d, it defaults to 128
bytes.) If the buffer is not large enough for the name and suffix, it
will truncate after `size - 1` bytes, to ensure that it is properly
`\0`-terminated.

The name suffix pointer is cleared after each `RUN_TEST*` call, so a
suffix can be constructed in a stack allocated buffer without later
dereferencing a pointer that has gone out of scope.


## Sub-Functions

Because of how `PASS()`, `ASSERT()`, `FAIL()`, etc. are implemented
(returning a test result enum value), calls to functions that use them
directly from test functions must be wrapped in `CHECK_CALL`:

    TEST example_using_subfunctions(void) {
        CHECK_CALL(less_than_three(5));
        PASS();
    }

This is only necessary if the called function can cause test failures.
The function should have a return type of `enum greatest_test_res`.


## Command Line Options

Test runners build with the following command line options:

    Usage: (test_runner) [-hlfave] [-s SUITE] [-t TEST] [-x EXCLUDE]
      -h, --help  print this Help
      -l          List suites and tests, then exit (dry run)
      -f          Stop runner after first failure
      -a          Abort on first failure (implies -f)
      -v          Verbose output
      -s SUITE    only run suite w/ name containing substring SUITE
      -t TEST     only run test w/ name containing substring TEST
      -e          only run exact name match for -s or -t
      -x EXCLUDE  exclude tests containing string substring EXCLUDE

Any arguments after `--` will be ignored.

If you want to run multiple test suites in parallel, look at
[parade](https://github.com/silentbicycle/parade).

These command line options are processed by `GREATEST_MAIN_BEGIN();`.


## Running Tests In Another Program

Rather than producing a command line test runner (which checks the
command line options, and exits with a pass/fail return code after
running tests), greatest can be used more like a library. Instead of
using `GREATEST_MAIN_BEGIN()`, use `GREATEST_INIT()` to (re-)initialize
greatest, then use either `GREATEST_PRINT_REPORT()` to print the report
to `GREATEST_STDOUT`, or use `greatest_get_report(&report)` to get the
pass, fail, skip, and assertion counters.

The command line flags above have corresponding functions:

- `greatest_stop_at_first_fail()`
- `greatest_abort_on_fail()`
- `greatest_list_only()`
- `greatest_set_exact_name_match()`
- `greatest_set_suite_filter(const char *filter)`
- `greatest_set_test_filter(const char *filter)`
- `greatest_set_test_exclude(const char *filter)`
- `greatest_get_verbosity()`
- `greatest_set_verbosity(unsigned int verbosity)`


## Aliases

Most of the macros have prefixed and unprefixed forms. For example,
`SUITE` is the same as `GREATEST_SUITE`.

Check the source for the list -- search for `GREATEST_USE_ABBREVS`.

These aliases can be disabled by `#define`-ing `GREATEST_USE_ABBREVS` to 0.


## Color Output

If you want color output (`PASS` in green, `FAIL` in red, etc.), you can
pipe the output through the included `greenest` script in `contrib/`:

```sh
$ ./example -v | greenest
```

(Note that `greenest` depends on a Unix-like environment.)

greatest itself doesn't have built-in coloring to stay small and portable.


## TAP Format

There is an awk script provided, `contrib/entapment`, that converts the
verbose output from the default CLI test runner to TAP version 13
format:

    ./example -v | contrib/entapment
