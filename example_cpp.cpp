#include <iostream>
#include "greatest.h"

using namespace std;

TEST standalone_test(void) {
    FAILm("(expected failure)");
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();
    RUN_TEST(standalone_test);
    GREATEST_MAIN_END();        /* display results */
    return 0;
}
