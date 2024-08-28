#include "libft.h"

#include "unity.h"

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define N_TRIALS 420

static void try_bzero(size_t size)
{
    if (size == 0)
        TEST_PASS();

    t_byte* actual = malloc(size);
    scramble_array(actual, size);
    ft_bzero(actual, size);

    t_byte* expected = malloc(size);
    scramble_array(expected, size);
    bzero(expected, size);

    compare_bytes(expected, actual, size);

    free(expected);
    free(actual);
}

static void test_stochastic_bzero(void)
{
    for (int i = 0; i < N_TRIALS; i++)
        try_bzero(random_size());
}

void run_test_bzero(void)
{
    printf("\n----- Bzero -----\n");
    RUN_TEST(test_stochastic_bzero);
}
