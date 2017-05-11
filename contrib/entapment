#!/usr/bin/awk -f
######################################################################
# Copyright (c) 2017 Scott Vokes <vokes.s@gmail.com>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
# 
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
######################################################################
# 
# This script converts greatest verbose output to TAP version 13.
#
# Usage:
#     test_runner -v | contrib/entapment
#     
######################################################################

BEGIN {
    diag_i = 1
    
    PASS = 0
    FAIL = 1
    SKIP = 2
    TODO = 3
}

/^\* Suite/ {
    suites[total_count] = $3
}

/^PASS/ {
    total_count++
    pass_count++
    results[total_count] = PASS
    $1 = ""
    msg[total_count] = $0
    next
}

/^FAIL/ {
    total_count++
    fail_count++
    results[total_count] = FAIL
    $1 = ""
    msg[total_count] = $0
    next
}

/^SKIP/ {
    total_count++
    if (match($0, "TODO")) {
        results[total_count] = TODO
    } else {
        results[total_count] = SKIP
    }
    $1 = ""
    msg[total_count] = $0
    next
}

{
    # Shift this by 1 so any test output is printed after ok/not ok line
    if (diag_groups[total_count + 1] == 0) {
        diag_groups[total_count + 1] = diag_lines
    }
    diag[diag_lines++] = $0
}

END {
    diag_groups[total_count] = diag_lines

    printf("TAP version 13\n")
    printf("%d..%d\n", 1, total_count)

    for (i = 1; i <= total_count; i++) {
        if (suites[i] != "") {
            printf("# suite %s\n", suites[i])
        }
        if (results[i] == PASS) {
            printf("ok %d -%s\n", i, msg[i])
        } else if (results[i] == FAIL) {
            printf("not ok %d -%s\n", i, msg[i])
        } else if (results[i] == SKIP) {
            printf("not ok %d # SKIP -%s\n", i, msg[i])
        } else if (results[i] == TODO) {
            printf("not ok %d # TODO -%s\n", i, msg[i])
        }

        if (diag_groups[i]) {
            for (d = diag_groups[i]; d < diag_groups[i + 1]; d++) {
                printf("# %s\n", diag[d])
            }
        }
    }
    if (total_count > 0) {
        printf("# pass %d, fail %d, skip %d, %.2f%% okay\n",
            pass_count, fail_count, total_count - (pass_count + fail_count),
            100 * (pass_count / total_count))
    }
}
