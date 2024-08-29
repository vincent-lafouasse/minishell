#include "libft.h"
#include "unity.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <bsd/string.h>
#endif

#define BUFFER_SIZE 1024

static void compare_strlcat_non_zero(const char* dest,
                                     const char* src,
                                     size_t buffer_size)
{
    char* my_buffer = malloc(buffer_size);
    char* libc_buffer = malloc(buffer_size);
    strlcpy(my_buffer, dest, buffer_size);
    strlcpy(libc_buffer, dest, buffer_size);

    size_t my_return = ft_strlcat(my_buffer, src, buffer_size);
    size_t libc_return = strlcat(my_buffer, src, buffer_size);

    char error[BUFFER_SIZE];

    sprintf(
        error,
        "Error with return value with input \"%s\", \"%s\" and %zu, ft_strlcat "
        "returned %zu and strlcat returned %zu",
        dest, src, buffer_size, my_return, libc_return);
    TEST_ASSERT_TRUE_MESSAGE(my_return == libc_return, error);

    compare_strings(libc_buffer, my_buffer);

    free(my_buffer);
    free(libc_buffer);
}

static void compare_strlcat_zero_buffer(const char* dest, const char* src)
{
    size_t buffer_size = 0;
    char* my_buffer = strdup(dest);
    char* libc_buffer = strdup(dest);

    size_t my_return = ft_strlcat(my_buffer, src, buffer_size);
    size_t libc_return = strlcat(libc_buffer, src, buffer_size);

    TEST_ASSERT_EQUAL_STRING(libc_buffer, my_buffer);

    char error[BUFFER_SIZE];

    sprintf(
        error,
        "Error with return value with input \"%s\", \"%s\" and %zu, ft_strlcat "
        "returned %zu and strlcat returned %zu",
        dest, src, buffer_size, my_return, libc_return);
    TEST_ASSERT_TRUE_MESSAGE(my_return == libc_return, error);

    free(my_buffer);
    free(libc_buffer);
}

static void test_strlcat(void)
{
    compare_strlcat_zero_buffer("aaa", "bb");
    compare_strlcat_non_zero("aaa", "bb", 1);
    compare_strlcat_non_zero("aaa", "bb", 2);
    compare_strlcat_non_zero("aaa", "bb", 3);
    compare_strlcat_non_zero("aaa", "bb", 4);
    compare_strlcat_non_zero("aaa", "bb", 5);
    compare_strlcat_non_zero("aaa", "bb", 5);
    compare_strlcat_non_zero("aaa", "bb", 6);
    compare_strlcat_non_zero("aaa", "bb", 7);
    compare_strlcat_non_zero("aaa", "bb", 8);
}

static void test_strlcat_unterminated_buffer(void)
{
    const size_t buffer_size = 10;

    char src[] = "yo";
    char my_dest[200];
    char libc_dest[200];
    memset(my_dest, 'a', 200);
    memset(libc_dest, 'a', 200);

    size_t my_return = ft_strlcat(my_dest, src, buffer_size);
    size_t libc_return = strlcat(libc_dest, src, buffer_size);

    char error[BUFFER_SIZE];

    sprintf(
        error,
        "Error with return value with unterminated dest and %zu, ft_strlcat "
        "returned %zu and strlcat returned %zu",
        buffer_size, my_return, libc_return);
    TEST_ASSERT_TRUE_MESSAGE(my_return == libc_return, error);

    compare_bytes((t_byte*)libc_dest, (t_byte*)my_dest, buffer_size);
}

void run_test_strlcat(void)
{
    printf("\n----- Strlcat -----\n");
    RUN_TEST(test_strlcat);
    RUN_TEST(test_strlcat_unterminated_buffer);
}
