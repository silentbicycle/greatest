# greatest

A testing system for C, contained in 1 file.


## Key Features

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
    share common setup, and can be in distinct compilation
    units.

- **Low Friction**

    Specific tests or suites can be run by name, for focused and rapid
    iteration during development. greatest adds very little startup
    latency.


There are some compile-time options, and slightly nicer syntax for
parametric testing (running tests with arguments) if compiled
with a C99 or later language standard.

I wrote a
[blog post](http://spin.atomicobject.com/2013/07/31/greatest-c-testing-embedded/)
with more information.

[theft][], a related project, adds [property-based testing][pbt].

[1]: http://spin.atomicobject.com/2013/07/31/greatest-c-testing-embedded/
[theft]: https://github.com/silentbicycle/theft
[pbt]: https://spin.atomicobject.com/2014/09/17/property-based-testing-c/
[ISC]: https://opensource.org/licenses/isc-license.txt

## Basic Usage

```c
#include "greatest.h"

/* A test runs various assertions, then calls PASS(), FAIL(), or SKIP(). */
TEST x_should_equal_1(void) {
    int x = 1;
    ASSERT_EQ(1, x);                              /* default message */
    ASSERT_EQm("yikes, x doesn't equal 1", 1, x); /* custom message */
    /* printf expected and actual values as "%d" if they differ */
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

    /* Individual tests can be run directly. */
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

Test cases should call assertions and then end in `PASS()`, `SKIP()`,
`FAIL()`, or one of their message variants (e.g. `SKIPm("TODO");`).
If there are any test failures, the test runner will return 1,
otherwise it will return 0. (Skips do not cause a test runner to
report failure.)

Tests and suites are just functions, so normal C scoping rules apply.
For example, a test or suite named "main" will have a name collision.

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


## Available Assertions

Assertions fail the current test unless some condition holds. All
assertions have a "message" variant (with an `m` suffix), which takes a
custom failure message string as their first argument. For example, the
assertion `ASSERT_EQ(apple, orange);` could instead be used like
`ASSERT_EQm("these should match", apple, orange)`. Non-message
assertions create a default message.


### `ASSERT(COND)`

Assert that `COND` evaluates to a true value.


### `ASSERT_FALSE(COND)`

Assert that `COND` evaluates to a false value.


### `ASSERT_EQ(EXPECTED, ACTUAL)`

Assert that `EXPECTED == ACTUAL`. To compare with a custom equality test
function, use `ASSERT_EQUAL_T` instead. To print the values if they
differ, use `ASSERT_EQ_FMT`.


### `ASSERT_EQ_FMT(EXPECTED, ACTUAL, FORMAT)`

Assert that `EXPECTED == ACTUAL`. If they are not equal, print their
values using FORMAT as the `printf` format string.

For example: `ASSERT_EQ_FMT(123, result, "%d");`

Note: `EXPECTED` and `ACTUAL` will be evaluated more than once on
failure, so they should not be a function call with side effects.
(Since their type is not known by the macro, they cannot be
captured in a local variable.)


### `ASSERT_IN_RANGE(EXPECTED, ACTUAL, TOLERANCE)`

Assert that ACTUAL is within EXPECTED +/- TOLERANCE, once the values
have been converted to a configurable floating point type
(`GREATEST_FLOAT`).


### `ASSERT_STR_EQ(EXPECTED, ACTUAL)`

Assert that the strings are equal
(i.e., `strcmp(EXPECTED, ACTUAL) == 0`).


### `ASSERT_STRN_EQ(EXPECTED, ACTUAL, SIZE)`

Assert that the first SIZE bytes of the strings are equal
(i.e., `strncmp(EXPECTED, ACTUAL, SIZE) == 0`).


### `ASSERT_MEM_EQ(EXPECTED, ACTUAL, SIZE)`

Assert that the first SIZE bytes of memory pointed to
by EXPECTED and ACTUAL are equal. If the memory differs, print
a hexdump and highlight the lines and individual bytes which
do not match.


### `ASSERT_ENUM_EQ(EXPECTED, ACTUAL, ENUM_STR_FUN)`

Assert that the enum value EXPECTED is equal to ACTUAL. If not, convert
each enum value to a string using `ENUM_STR_FUN` before printing them.

`ENUM_STR_FUN` should have a type like:

    const char *some_enum_str(enum some_enum x);


### `ASSERT_EQUAL_T(EXPECTED, ACTUAL, TYPE_INFO, UDATA)`

Assert that EXPECTED and ACTUAL are equal, using the `greatest_equal_cb`
function pointed to by `TYPE_INFO->equal` to compare them. The
assertion's `UDATA` argument can be used to pass in arbitrary user data
(or `NULL`). If the values are not equal and the `TYPE_INFO->print`
function is defined, it will be used to print an "Expected: X, Got: Y"
message.


### `ASSERT_OR_LONGJMP(COND)`

Assert that `COND` evaluates to a true value. If not, then use
`longjmp(3)` to immediately return from the test case and any
intermediate function calls. (If built with `GREATEST_USE_LONGJMP`
defined to 0, then all setjmp/longjmp-related functionality will be
compiled out.)


## Random Shuffling

Groups of suites or tests can be run in random order by using
`GREATEST_SHUFFLE_SUITES` and `GREATEST_SHUFFLE_TESTS`, respectively.
This can help find and eliminate coupling between tests.

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
`SHUFFLE_SUITES` block (without a suite), because the test will run over
and over.


## Sub-Functions

Because of how `PASS()`, `ASSERT()`, `FAIL()`, etc. are implemented
(returning a test result enum value), calls to functions that use them
directly from test functions must be wrapped in `CHECK_CALL`:

    TEST example_using_subfunctions(void) {
        CHECK_CALL(less_than_three(5));
        PASS();
    }

This is only necessary if the called function can cause test failures.


## Command Line Options

Test runners build with the following command line options:

    Usage: (test_runner) [--help] [-hlfv] [-s SUITE] [-t TEST]
      -h, --help  print this Help
      -l          List suites and tests, then exit (dry run)
      -f          Stop runner after first failure
      -v          Verbose output
      -s SUITE    only run suite w/ name containing SUITE substring
      -t TEST     only run test w/ name containing TEST substring
      -t EXCLUDE  exclude tests containing string EXCLUDE substring

Any arguments after `--` will be ignored.

If you want to run multiple test suites in parallel, look at
[parade](https://github.com/silentbicycle/parade).

These command line options are processed by `GREATEST_MAIN_BEGIN();`.


## Aliases

Most of the macros have prefixed and unprefixed forms. For example,
`SUITE` is the same as `GREATEST_SUITE`.

Check the source for the list -- search for `#if GREATEST_USE_ABBREVS`.

These aliases can be disabled by `#define`-ing `GREATEST_USE_ABBREVS` to 0.


## Color Output

If you want color output (`PASS` in green, `FAIL` in red, etc.), you can
pipe the output through the included `greenest` script in `contrib/`:

```sh
$ ./example -v | greenest
```

(Note that `greenest` depends on a Unix-like environment.)

greatest itself doesn't have built-in coloring to stay small and portable.
