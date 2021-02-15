# greatest Changes By Release

## v1.5.0 - 2021-02-15

### API Changes

Changed default: `GREATEST_USE_LONGJMP` now defaults to 0. This eliminates
a warning about variables that can possibly become stale/corrupt in the
presence of `longjmp`. Since `GREATEST_FAIL_WITH_LONGJMP` isn't frequently
used, it should probably be opt-in.

Added `greatest_set_exact_name_match()` / `-e` flag, which changes the
name-based filtering from substring to exact match. Note that filtering
on an exact suite name will not skip tests run outside of any suite.

Added `GREATEST_ASSERT_NEQ` and `GREATEST_ASSERT_NEQm`. (Thanks @tekknolagi.)

Added `GREATEST_ASSERT_GT`, `GREATEST_ASSERT_GTE`, `GREATEST_ASSERT_LT`,
and `GREATEST_ASSERT_LTE`, along with their custom message (`m`)
variants.


### Bug Fixes

Makefile: Fix targets so all files are rebuilt when `greatest.h` or the
`Makefile` are modified, but without potentially breaking the build due
to including `greatest.h` as a linker argument to `example_trunc` (which
could happen with clang). (Thanks @vemakereporter, @theosotr.)

Calls to `GREATEST_RUN_TEST` from inside another test are now ignored.

Other flags starting with `--` besides `--help` (print help) and `--`
(ignore rest of ARGV) now produce an "Unknown argument" message;
previously they were unintentionally handled like `--`.


### Other Improvements

Added built `example_cpp` executable to `.gitignore`.

Expanded on the role of `RUN_TEST`, `RUN_TEST1`, `RUN_TESTp`,
`PASS`, `SKIP`, and `FAIL` in the README.

Addressed a `-Wimplicit-fallthrough` warning when building with clang
using `-Weverything`.


## v1.4.2 - 2019-03-24

### API Changes

None.


### Other Improvements

Fixed configuration parameters for the PRNG (used for shuffling).
Previously, certain rare combinations of test counts and input states
could prevent the linear congruential RNG from having its full period
before repeating values, which could cause tests to be run multiple
times or skipped during shuffling.

Fixed an assertion in the example code where the expected annd actual
parameters were swapped. (Reported by @shaohuasong.)

Updated a comment to reflect that `GREATEST_ASSERT_STR_EQm` compares
using `strncmp`, not `strcmp`. (Reported by @orangewait.)

Minor formatting improvements in the README.


## v1.4.1 - 2018-12-30

### API Changes

None.


### Other Improvements

Fixed a warning when compiled with `-Wconversion`. (Thanks @HawkSK.)

Replaced a remaining direct use of `fprintf` with `GREATEST_FPRINTF`.
(Thanks @ligurio.)

Fixed typo in example code. (Thanks @ligurio.)

Bugfix: Previously, a test failure in first-fail mode didn't prevent
later suites from running.

Added a comment noting that the duplicated declaration inside the
`GREATEST_SUITE` macro is intentional.

Renamed `exp` local variable to `expd`, to prevent a possible
warning about shadowing `exp(3)`.

Updated the command line argument documentation in the README and
the usage message.

Added a description of output behaviors for each test case
result type, and clarified that the custom message variants
(e.g. `SKIPm("TODO")`) can be used to set custom messages, but
do not affect whether or not the message is printed.


## v1.4.0 - 2018-03-05

### API Changes

Added `greatest_abort_on_fail`, which sets a flag to call `abort()` on
the first test failure (`-a` in the CLI test runner). If tests are
running inside a debugger, this will break right on the failure.
Feature suggestion and initial implementation by @fsaintjacques.

Added `greatest_list_only()`, which sets the same flag as the `-l`
option in the CLI test runner.

Added `greatest_set_test_suffix(const char *suffix)`, which can
be used to add a suffix to the name printed for the next test.
This can be used to distinguish between tests when running
parametric tests (particularly when shuffled). Note that this
suffix is included in the matching for `-t` and `-x`.

The `greatest_info` struct now allocates a `char` buffer for the test
name and optional '_' separator & suffix. The buffer size can be
configured by `#define`ing `GREATEST_TESTNAME_BUF_SIZE`. (See
`example_trunc.c`.) If the test name plus optional suffix does not fit
in the buffer, it will be truncated and `\0` terminated.

Made a couple functions `static` that were previously exposed, but
explictly listed as being internal (`greatest_do_pass`,
`greatest_do_fail`, `greatest_do_skip`, `greatest_suite_pre`,
`greatest_suite_post`, `greatest_usage`).


### Other Improvements

Fixed link to ISC license in README.md. (Thanks @vaibhavsagar.)

Fixed issue link and whitespace in README.md and CONTRIBUTING.md.
(Thanks @bebehei.)

Change a couple macros into functions: `GREATEST_INIT` and
`GREATEST_PRINT_REPORT`. Most of the macros have variable capture,
return from their call site, etc., but these two don't need to be
macros.


## v1.3.1 - 2017-09-22

### API Changes

None.


### Other Improvements

Bugfix: `GREATEST_SHUFFLE_TESTS` and `GREATEST_SHUFFLE_SUITES`
did not check for the `stop_at_first_fail` flag, and could get
stuck in an infinite loop. Now their loops stop as expected.

Renamed `example_random.c` to `example_shuffle.c`, since its
focus is really on random shuffling of suites and tests.


## v1.3.0 - 2017-08-13

### API Changes

Added `GREATEST_SHUFFLE_SUITES(SEED, BODY)` macro (and `SHUFFLE_SUITES`
abbreviation). This runs any suites within BODY in pseudorandom order,
seeded by SEED.

Added `GREATEST_SHUFFLE_TESTS(SEED, BODY)` macro (and `SHUFFLE_TESTS`
abbreviation). This runs any tests within BODY in pseudorandom order,
seeded by SEED.

If `GREATEST_NO_EXTERN_CPLUSPLUS` is `#define`d, then the C++
`extern "C" { ... }` namespacing around greatest is disabled.

All calls to `fprintf` have been wrapped in a macro, `GREATEST_FPRINTF`,
which can be `#define`d to substitute another function with the same
interface. Feature suggestion and initial implementation by
@AndreasMartin72.

Added `greatest_stop_at_first_fail()`, to set the flag to stop the test
runner at the first failure (`-f` in the CLI test runner).

Added `void greatest_test_exclude(const char *filter)`, which takes a
filter string and ignores all tests whose names contain the filter
string. This takes precedence over `greatest_test_filter` matches.

Added a CLI test runner option, `-x`, which calls
`greatest_test_exclude` with its argument.


### Other Improvements

Bugfix: `GREATEST_SAVE_CONTEXT()` was only used in `GREATEST_RUN_TEST`,
not `GREATEST_RUN_TEST1` or `GREATEST_RUN_TESTp`, which could cause
corruption when tests using them called `ASSERT_OR_LONGJMPm` or
`FAIL_WITH_LONGJMPm`. Reported and fixed by @tcarmelveilleux.

Add `contrib/entapment` script, which converts a greatest test runner's
verbose-style output to TAP version 13 format.

Add `contrib/testify` script, which converts calls to `RUN_TEST(test);`
to test functions with `SKIPm("TODO");`.

Disabled `-Weverything` in Makefile: it isn't portable.

Various improvements to the documentation.

If list (`-l`) and test name filtering are both used, only tests that
would have run are printed. This can be used to test filter strings.


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
