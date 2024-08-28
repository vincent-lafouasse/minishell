#include "libft.h"
#include "unity.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void compare_strdup(const char* src)
{
    char* my_buffer = ft_strdup(src);
    char* libc_buffer = strdup(src);

    compare_bytes((t_byte*)my_buffer, (t_byte*)libc_buffer, strlen(src));

    free(my_buffer);
    free(libc_buffer);
}

static void test_strdup(void)
{
    compare_strdup("SOME");
    compare_strdup("");
    compare_strdup("body once told me the world is gonna roll me");
    compare_strdup("I ain't the sharpest tool in the shed");
    compare_strdup(
        "She was looking kind of dumb with her finger and her thumb");
    compare_strdup("In the shape of an \"L\" on her forehead");
}

void run_test_strdup(void)
{
    printf("\n----- Strdup -----\n");
    RUN_TEST(test_strdup);
}
