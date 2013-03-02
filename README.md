A unit testing system for C, contained in 1 file. It doesn't use dynamic
allocation or depend on anything beyond ANSI C89, and the test scaffolding
should build without warnings under `-Wall -pedantic`.

To use, just include greatest.h in your project. Note that there are
some compile time options, and additional functionality (parametric
testing) is available if compiled with -std=c99.

For usage examples, look at example.c and example-suite.c.

    Usage: (test_runner) [-hlfv] [-s SUITE] [-t TEST]
      -h        print this Help
      -l        List suites and their tests, then exit
      -f        Stop runner after first failure
      -v        Verbose output
      -s SUITE  only run suite named SUITE
      -t TEST   only run test named TEST
