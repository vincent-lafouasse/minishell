#include "libft.h"

#include "unity.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

static void compare_atoi(const char* s)
{
    int expected = atoi(s);
    int actual = ft_atoi(s);

    char error[BUFFER_SIZE];
    sprintf(error, "Error for input \"%s\", expected %d was %d", s, expected,
            actual);

    TEST_ASSERT_TRUE_MESSAGE(expected == actual, error);
}

static void compare_atoi_at_extrema(void)
{
    char int_max[BUFFER_SIZE];
    char int_min[BUFFER_SIZE];

    sprintf(int_max, "%d", INT_MAX);
    sprintf(int_min, "%d", INT_MIN);

    compare_atoi(int_max);
    compare_atoi(int_min);
}

static void test_atoi_valid(void)
{
    compare_atoi("0");
    compare_atoi("1");
    compare_atoi("42");
    compare_atoi("+42");
    compare_atoi("-42");
    compare_atoi("              \t\n\t 42");
    compare_atoi("-1");
    compare_atoi_at_extrema();
}

static void test_atoi_invalid(void)
{
    compare_atoi("42born2code");
    compare_atoi("yo yoyo what uppppp");
    compare_atoi("42 c'est pas mal en fait");
    compare_atoi("0xcafe");
    compare_atoi("--------42");
    compare_atoi("--+--+---+-42born2code");
}

void run_test_atoi(void)
{
    printf("\n----- Atoi -----\n");
    RUN_TEST(test_atoi_valid);
    RUN_TEST(test_atoi_invalid);
}
