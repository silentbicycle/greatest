#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"

TEST blah() {
    PASS();
}

GREATEST_SUITE(other_suite) {
    RUN_TEST(blah);
}
