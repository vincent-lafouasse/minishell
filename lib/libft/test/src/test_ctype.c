#include "libft.h"

#include "unity.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

#define LOWER_BOUND -1
#define UPPER_BOUND 255

static void compare_as_int(int (*my_ft)(int), int (*libc_ft)(int), int c)
{
    int expected = (*libc_ft)(c);
    int actual = (*my_ft)(c);

    char error[BUFFER_SIZE];
    sprintf(error, "Error for input %c, ie ASCII %d, expected %d was %d", c, c,
            expected, actual);

    TEST_ASSERT_TRUE_MESSAGE(expected == actual, error);
}

static void compare_as_bool(int (*my_ft)(int), int (*libc_ft)(int), int c)
{
    bool expected = (*libc_ft)(c);
    bool actual = (*my_ft)(c);

    char error[BUFFER_SIZE];
    sprintf(error, "Error for input %c, ie ASCII %d, expected %d was %d", c, c,
            expected, actual);

    TEST_ASSERT_TRUE_MESSAGE(expected == actual, error);
}

/*
static void test_islower(void)
{
    for (int c = LOWER_BOUND; c <= UPPER_BOUND; c++)
        compare_as_bool(&ft_islower, &islower, c);
}

static void test_isupper(void)
{
    for (int c = LOWER_BOUND; c <= UPPER_BOUND; c++)
        compare_as_bool(&ft_isupper, &isupper, c);
}

*/
static void test_isdigit(void)
{
    for (int c = LOWER_BOUND; c <= UPPER_BOUND; c++)
        compare_as_bool(&ft_isdigit, &isdigit, c);
}

static void test_isascii(void)
{
    for (int c = LOWER_BOUND; c <= UPPER_BOUND; c++)
        compare_as_bool(&ft_isascii, &isascii, c);
}

static void test_isprint(void)
{
    for (int c = LOWER_BOUND; c <= UPPER_BOUND; c++)
        compare_as_bool(&ft_isprint, &isprint, c);
}

static void test_isalpha(void)
{
    for (int c = LOWER_BOUND; c <= UPPER_BOUND; c++)
        compare_as_bool(&ft_isalpha, &isalpha, c);
}

static void test_isalnum(void)
{
    for (int c = LOWER_BOUND; c <= UPPER_BOUND; c++)
        compare_as_bool(&ft_isalnum, &isalnum, c);
}

/*
static void test_isspace(void)
{
    for (int c = LOWER_BOUND; c <= UPPER_BOUND; c++)
        compare_as_bool(&ft_isspace, &isspace, c);
}
*/

static void test_toupper(void)
{
    for (int c = LOWER_BOUND; c <= UPPER_BOUND; c++)
        compare_as_int(&ft_toupper, &toupper, c);
}

static void test_tolower(void)
{
    for (int c = LOWER_BOUND; c <= UPPER_BOUND; c++)
        compare_as_int(&ft_tolower, &tolower, c);
}

void run_test_ctype(void)
{
    printf("\n----- Ctype -----\n");
    RUN_TEST(test_isdigit);
    RUN_TEST(test_isprint);
    RUN_TEST(test_isalpha);
    RUN_TEST(test_isalnum);
    // RUN_TEST(test_islower);
    // RUN_TEST(test_isupper);
    // RUN_TEST(test_isspace);
    RUN_TEST(test_toupper);
    RUN_TEST(test_tolower);
    RUN_TEST(test_isascii);
}
