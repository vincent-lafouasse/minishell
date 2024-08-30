#include "libft.h"
#include "unity.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

static void compare_strncmp(const char* s1, const char* s2, size_t n)
{
    int my_res = ft_strncmp(s1, s2, n);
    int libc_res = strncmp(s1, s2, n);

    char error[BUFFER_SIZE];
    sprintf(error,
            "Error for input \"%s\", \"%s\", %zu. ft_strncmp returned %d while "
            "strncmp returned %d",
            s1, s2, n, my_res, libc_res);

    TEST_ASSERT_TRUE_MESSAGE(sign(my_res) == sign(libc_res), error);
}

static void test_strncmp(void)
{
    compare_strncmp("aaa", "aab", 0);
    compare_strncmp("aaa", "aab", 1);
    compare_strncmp("aaa", "aab", 2);
    compare_strncmp("aaa", "aab", 3);
    compare_strncmp("aaa", "zzz", 0);
    compare_strncmp("aaa", "", 0);
}

void run_test_strncmp(void)
{
    printf("\n----- Strncmp -----\n");
    RUN_TEST(test_strncmp);
}
