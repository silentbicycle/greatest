/* Make the buffer size VERY small, to check truncation */
#define GREATEST_TESTNAME_BUF_SIZE 8

#include "greatest.h"

TEST t(void) {
    PASS();
}

TEST abcdefghijklmnopqrstuvwxyz(void) {
    PASS();
}

SUITE(suite) {
    size_t i;
    char buf[10];
    memset(buf, 0x00, sizeof(buf));
    for (i = 0; i < sizeof(buf); i++) {
        if (i > 0) { buf[i - 1] = 'x'; }
        greatest_set_test_suffix(buf);
        RUN_TEST(t);
    }

    RUN_TEST(abcdefghijklmnopqrstuvwxyz);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);

    printf("sizeof(greatest_info): %lu\n",
        (unsigned long)sizeof(greatest_info));
    GREATEST_MAIN_END();
}
