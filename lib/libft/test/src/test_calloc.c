#include "libft.h"

#include "unity.h"

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define N_TRIALS 420

static void try_calloc(size_t count, size_t size)
{
    if (size == 0)
        TEST_PASS();

    t_byte* actual = calloc(count, size);
    t_byte* expected = calloc(count, size);

    compare_bytes(expected, actual, count * size);

    free(expected);
    free(actual);
}

static void test_stochastic_calloc(void)
{
    for (int i = 0; i < N_TRIALS; i++)
        try_calloc(random_size(), random_size());
}

void run_test_calloc(void)
{
    printf("\n----- Calloc -----\n");
    RUN_TEST(test_stochastic_calloc);
}
