#include "libft.h"

#include "unity.h"

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define N_TRIALS 420

static void try_memset(int value, size_t size)
{
    if (size == 0)
        TEST_PASS();

    t_byte* actual = malloc(size);
    scramble_array(actual, size);
    ft_memset(actual, value, size);

    t_byte* expected = malloc(size);
    scramble_array(expected, size);
    memset(expected, value, size);

    compare_bytes(expected, actual, size);

    free(expected);
    free(actual);
}

static void test_memset(void)
{
    try_memset(0, 0);
    try_memset(69, 420);
}

static void test_stochastic_memset(void)
{
    for (int i = 0; i < N_TRIALS; i++)
        try_memset(random_int(), random_size());
}

void run_test_memset(void)
{
    printf("\n----- Memset -----\n");
    RUN_TEST(test_memset);
    RUN_TEST(test_stochastic_memset);
}
