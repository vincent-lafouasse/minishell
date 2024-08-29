#include "libft.h"
#include "unity.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

static void compare_memcmp(const void* s1, const void* s2, size_t n)
{
    int my_res = ft_memcmp(s1, s2, n);
    int libc_res = memcmp(s1, s2, n);

    char error[BUFFER_SIZE];
    sprintf(error,
            "Error for input \"%s\", \"%s\", %zu. ft_memcmp returned %d while "
            "memcmp returned %d",
            (t_byte*)s1, (t_byte*)s2, n, my_res, libc_res);

    TEST_ASSERT_TRUE_MESSAGE(sign(my_res) == sign(libc_res), error);
}

static void test_memcmp(void)
{
    compare_memcmp("aaa", "aab", 0);
    compare_memcmp("aaa", "aab", 1);
    compare_memcmp("aaa", "aab", 2);
    compare_memcmp("aaa", "aab", 3);
}

void run_test_memcmp(void)
{
    printf("\n----- Memcmp -----\n");
    RUN_TEST(test_memcmp);
}
