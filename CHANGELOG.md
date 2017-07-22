# greatest Changes By Release

## v_._._ - TBD

### API Changes

Added `GREATEST_SHUFFLE_TESTS(SEED, BODY)` macro (and `SHUFFLE_TESTS`
abbreviation). This runs any tests within BODY in pseudorandom order,
seeded by SEED.


### Other Improvements

Add `contrib/entapment` script, which converts a greatest test runner's
verbose-style output to TAP version 13 format.

Disabled `-Weverything` in Makefile: it isn't portable.


## v1.2.2 - 2017-04-26

### API Changes

None.


### Other Improvements

Explicitly mention ISC License in README.

Always `fflush(GREATEST_STDOUT)` after tests, rather than checking
whether `GREATEST_STDOUT` expands to `stdout` (the default) first. This
avoids a tautological comparison warning. Reported by @jibsen and
@nemequ.

Address warnings that can appear under `-Weverything`: padding in
`struct greatest_run_info`, a pointer cast that unintentionally removed
`const`, and a non-literal `printf` format string (caused by a literal
format string constructed and saved to a variable in a macro). Reported
by @kozross.


## v1.2.1 - 2016-06-25

### API Changes

None.


### Other Improvements

Accept "--help" as an alternate form of "-h" in command line test
runners. (Thanks @jibsen).

Avoid redundant evaluation of arguments in `ASSERT_ENUM_EQ`.

Clarify type of `ENUM_STR` argument in `ASSERT_ENUM_EQ`. Use a function
pointer typedef so the compiler can give better type errors.

Warn about redundant evaluation of arguments in `ASSERT_EQ_FMT`.

Prefix the variables used in the expansion of the `ASSERT_EQ_FMT`,
`ASSERT_ENUM_EQ`, and `ASSERT_IN_RANGE` macros, to avoid name clashes
that can cause confusing compiler errors.


## v1.2.0 - 2016-06-06

### API Changes

Added `GREATEST_IGNORE_TEST`, which skips a test without warning about
the function being unused.

Added `GREATEST_ASSERT_STRN_EQ`, which does a string comparison with an
explicit length (using `strncmp`, rather than `strcmp`).

Added `GREATEST_ASSERT_MEM_EQ`, which does a memory comparison with an
explicit length (using `memcmp`). If the memory differs, it will print a
hexdump which highlights the lines and individual bytes which differ.
(The `greenest` script was also updated to colorize the hexdump's
difference markers.)

Added `GREATEST_ASSERT_ENUM_EQ`, which compares two values and uses an
enum-specific function to convert them to strings before printing
failures.

The `greenest` script has been moved into `contrib/`.


### Other Improvements

Fixed a bug that kept the `-f` (stop after first failure) flag from
stopping the test runner immediately when a failure occured just
before running a new suite.

Added `extern "C" { ... }` guards for improved C++ compatibility.

Fixed a warning when compiling as C++.

Several updates to the README.

A file with contribution guidelines (`CONTRIBUTING.md`) was added.


## v1.1.1 - 2016-02-14

### API Changes

Align "Expected:"/"Got:" messages, so it's easier to compare values.


### Other Improvements

Fixed a bug that could lead to test suites running previous suites'
setup/teardown callbacks. This was a bug in the optional suite support,
so only v1.1.0 is affected.

Updated the README to note that the `greenest` awk script expects
a Unix-like environment (i.e., it doesn't work on Windows).


## v1.1.0 - 2015-11-29

### API Changes

Made suites optional, so that `RUN_TEST` works outside of any suites.
This reduces boilerplate in cases where there would only be one "main"
suite anyway.

Added `greatest_get_report()`, to populate a struct with the pass, fail,
skip, and assertion counts, for cases where getting the counts directly
is preferable to printing them with `GREATEST_PRINT_REPORT()` or
`GREATEST_MAIN_END()` (i.e., without a CLI test runner).

Added `greatest_set_test_filter(substring)` and
`greatest_set_suite_filter(substring)`, so only tests or suites matching a
filter substring can be run without using the CLI test runner.

Added `greatest_set_flag()`, so the FIRST_FAIL flag can be set without
using the CLI test runner.

Added `SUITE_EXTERN` macro, for declaring suites in other compilation units.
They could previously be declared using the `SUITE` macro, but this made a
redundant declaration that could cause warnings.

Added multiple levels of verbosity. Can be checked and set with
`greatest_get_verbosity()`, and `greatest_set_verbosity()` respectively.
(`GREATEST_IS_VERBOSE()` remains in the API.)

Stop parsing CLI options if `--` is reached, so user code can include
its own afterward.


### Other Improvements

Added the "Key Features" section to the README, and updated the
tagline to reflect that greatest isn't specific to unit testing.

Added this changelog.

Added LICENSE file (ISC License), to match common project layouts.
greatest already had its license in the `greatest.h` header, but this
makes it easier to find at a glance.

Added a URL for finding the newest version of greatest to the header.

Updated the default report message: print "1 test", not "1 tests";
print "X passed, Y failed," instead of "X pass, Y fail,", etc.

Updated the CLI test runner's help message to reflect that the
`-s` and `-t` switches match by substring, not the full name.

Fixed C89 error due to incrementing assertion count before
declaring variables (mixing declarations and code).

Fixed warnings.


### Known Issues

The `greatest_printf_cb` callback in `struct greatest_type_info` type
should have an extra `FILE *` argument (i.e., behave like fprintf, not
printf). This leads to cases where `GREATEST_STDOUT` has been customized
and `GREATEST_ASSERT_EQUAL_T`'s "Expected:", "Got:" messages are printed
to the correct stream, but the callback still prints the values on
stdout. Resolving this is an interface change, so it will have to wait
until release v2.0.0. (The actual code change will be minor -- adding an
extra f argument to the callback, and passing it to fprinf rather than
using printf.)

The `greatest_type_info` struct is typedef'd, which is inconsistent with
the API otherwise not typedef-ing user facing structs. (It is also in the
struct namespace.) Fixing this will have to wait until v2.0.0. 


## v1.0.0 - 2015-05-11

### API Changes

Added `ASSERT_EQ_FMT(expected, actual, format)` to compare scalar values and
print them (if different) using the given format string (e.g. "0x%08x").

Added `ASSERT_IN_RANGE(expected, actual, tolerance)` to check whether the
actual value is within a +/- tolerance, as a floating point comparison,
and print the actual value if out of bounds. The floating point type used
can be configured by overriding GREATEST_FLOAT.
    
Added `GREATEST_USE_TIME` compile-time #define, which can be set to 0
to compile out all time.h / clock() related functionality.
    
Added check for `_MSC_VER`, to detect C99 compatibility when built with
Visual Studio.


## v0.11.1 - 2015-01-17

Patch version, increased due to bug fixes (thanks @pmj):
    
. Fix for issue #26 - must not longjmp to a context in an exited function.
. Corrected use of non-prefixed FAILm() macro in ASSERT macros.
. Avoid compile warnings about no previous definition of suite functions.


## v0.11.0 - 2015-01-02

### API Changes

Added `CHECK_CALL()` macro, to wrap a function call that can fail a test.

Added `ASSERT_OR_LONGJMP()` / `FAIL_WITH_LONGJMP()`, to longjmp(3) from
failing functions rather than returning. This is optional, and can be
compiled out by `#define`-ing `GREATEST_USE_LONGJMP` to 0.

Restructured API so a CLI test runner is no longer necessary.
`GREATEST_INIT()` and `GREATEST_PRINT_REPORT()` can be called directly.
    

### Other Improvements

Explicitly documented the protocol for calling functions from a test that
themselves call ASSERT(), SKIP(), FAIL(), etc. Added an enum for the
test return type.

Fixed warnings, improved the documentation.


## v0.10.1 - 2014-11-16

Fixed error when compiled as C++ by adding `extern` to
`greatest_type_info` struct.


## v0.10.0 - 2014-10-12

### API Changes

Added `ASSERT_EQUAL_T`, for comparing arbitrary structs and printing
them when they differ.


### Other Improvements

Added `greenest` awk script, to color output.


## v0.9.4 - 2014-08-06

Started counting the total number of assertions.

Fixed warnings.


## v0.9.3 - 2014-05-30

### API Changes

Changed filters for test (`-t`) and suite (`-s`) names to use substring
search, rather than exact matches.

Added `GREATEST_FAILURE_ABORT()`, to check whether the test runner is in
first-fail (`-f`) mode and there have been failures.


### Other Improvements

Added check to always print a line break for "FAIL" messages, unless at
column 0.


## v0.9.2 - 2013-08-06

### API Changes

Added a C89-compatible form of parametric testing (`GREATEST_RUN_TEST1`).


### Other Improvements

Fixed warnings when compiled as C++.


## v0.9.1 - 2013-07-14

Added `-f` to usage line in documentation.

Fixed warnings.


## v0.9.0 - 2013-03-02

### API Changes

Added finish-at-first-failure (`-f`) to CLI test runner.

Added suite-level setup/teardown hooks.


### Other Improvements

Fixed warnings.


## v0.8 - 2012-04-08

### API Changes

Added help (`-h`) and list (`-l`) options to CLI test runner.

Added `GREATEST_RUN_TESTp` for parametric testing in C99.

Added `GREATEST_IS_VERBOSE()`, to check whether the verbose (`-v`) flag
was set.

Added `GREATEST_SET_TEARDOWN_CB()`, for setting a teardown callback.

Added custom message forms of PASS, FAIL, SKIP, e.g. `PASSm()`.


### Other Improvements

Fixed warnings.


## Pre-v0.8 - 2012-01-09

Initial public release of project.
