#include "libft.h"

#include "unity.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

static void compare(char* (*myft)(const char*, int),
                    char* (*libcft)(const char*, int),
                    const char* s,
                    int c)
{
    char* expected = (*libcft)(s, c);
    char* actual = (*myft)(s, c);
    char error[BUFFER_SIZE];

    sprintf(error,
            "Error found with input \"%s\" and %c, expected was \"%s\", actual "
            "was \"%s\"",
            s, c, expected, actual);
    TEST_ASSERT_TRUE_MESSAGE(expected == actual, error);
}

static void compare_strchr(const char* s, int c)
{
    compare(&ft_strchr, &strchr, s, c);
}

static void compare_strrchr(const char* s, int c)
{
    compare(&ft_strrchr, &strrchr, s, c);
}

static void test_strchr(void)
{
    compare_strchr("Hello what is up", 'H');
    compare_strchr("Hello what is up", 'w');
    compare_strchr("Hello what is up", '\0');
}

static void test_strrchr(void)
{
    compare_strrchr("Hello what is up", 'H');
    compare_strrchr("Hello what is up", 'w');
    compare_strrchr("Hello what is up", '\0');
}

void run_test_strchr(void)
{
    printf("\n----- Strchr -----\n");
    RUN_TEST(test_strchr);
    RUN_TEST(test_strrchr);
}
