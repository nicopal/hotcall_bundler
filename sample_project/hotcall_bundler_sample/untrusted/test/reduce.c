#include "gtest/gtest.h"
#include "test.h"
#include "hotcall-bundler-untrusted.h"
#include "functions.h"

#include "hotcall_reduce.h"

TEST(reduce,1) {
    //Contract: res should be equal to the number of iterations.
    hotcall_test_setup();

    BUNDLE_BEGIN();

    unsigned int n_iters = 10;
    int xs[n_iters] = { 0 };
    int res = 0;

    //struct parameter vec1[] = { VAR(xs, 'd'), VECTOR_v2(&vec1[0], &n_iters) }, p1 = vec1[1];

    REDUCE(
        ((struct reduce_config) {
            .function_id = hotcall_ecall_plus_one_ret,
            .op = '+',
        }),
        VECTOR(xs, 'd', &n_iters), VAR(res, 'd')
    );

    BUNDLE_END();

    hotcall_test_teardown();

    ASSERT_EQ(res, n_iters);
}


TEST(reduce, 2) {
    //Contract: Should return the sum of each element (x + y) where x is a member of xs.
    hotcall_test_setup();

    unsigned int n_iters = 10;
    int xs[n_iters] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int res = 0, y = 5;

    //struct parameter vec1[] = { VAR(xs, 'd'), VECTOR_v2(&vec1[0], &n_iters) }, p1 = vec1[1];

    BUNDLE_BEGIN();

    REDUCE(
        ((struct reduce_config) {
            .function_id = hotcall_ecall_plus,
            .op = '+'
        }),
        VECTOR(xs, 'd', &n_iters), VAR(y, 'd'), VAR(res, 'd')
    );

    BUNDLE_END();

    hotcall_test_teardown();

    int sum = 0;
    for(int i = 0; i < n_iters; ++i) {
        sum += i + y;
    }
    ASSERT_EQ(res, 95);
}


TEST(reduce, 3) {
    //Contract: Should return true since all elements in xs are greater than 2
    hotcall_test_setup();

    BUNDLE_BEGIN();

    unsigned int n_iters = 7;
    int xs[n_iters] = { 3, 4, 5, 6, 7, 8, 9 };
    bool res;

    //struct parameter vec1[] = { VAR(xs, 'd'), VECTOR_v2(&vec1[0], &n_iters) }, p1 = vec1[1];

    REDUCE(
        ((struct reduce_config) {
            .function_id = hotcall_ecall_greater_than_two,
            .op = '&',
        }),
        VECTOR(xs, 'd', &n_iters), VAR(res, 'd')
    );

    BUNDLE_END();

    hotcall_test_teardown();

    ASSERT_EQ(res, true);
}

TEST(reduce, 4) {
    //Contract:
    hotcall_test_setup();

    BUNDLE_BEGIN();

    unsigned int n_iters = 7;
    int xs[n_iters] = { 10, 10, 10, 10, 10, 10, 10 };
    int res;


    REDUCE(
        ((struct reduce_config) {
            .function_id = 255,
            .op = '+',
        }),
        VECTOR(xs, 'd', &n_iters), VAR(res, 'd')
    );

    BUNDLE_END();

    hotcall_test_teardown();

    ASSERT_EQ(res, 70);
}
