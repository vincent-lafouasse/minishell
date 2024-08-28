#include "utils.h"

#include "unity.h"

#include <stdlib.h>

#define MAX_SIZE 1024
#define BUFFER_SIZE 1024

void compare_bytes(t_byte* expected_array, t_byte* actual_array, size_t size)
{
    t_byte expected;
    t_byte actual;
    char error[BUFFER_SIZE];

    for (size_t i = 0; i < size; i++)
    {
        expected = expected_array[i];
        actual = actual_array[i];

        if (expected != actual)
        {
            sprintf(error, "Error at byte %zu, expected 0x%02x was 0x%02x", i,
                    expected, actual);
            TEST_FAIL_MESSAGE(error);
        }
    }
}

void compare_strings(const char* expected, const char* actual)
{
    size_t i = 0;

    while (expected[i] == actual[i] && expected[i] && actual[i])
    {
        i++;
    }
    if (expected[i] == '\0' && actual[i] == '\0')
    {
        TEST_PASS();
    }

    char error[BUFFER_SIZE];
    sprintf(error,
            "Error, expected string \"%s\" and actual string \"%s\" are "
            "different at byte %zu. Expected %c was %c",
            expected, actual, i, expected[i], actual[i]);
    TEST_FAIL_MESSAGE(error);
}

void scramble_array(void* array, size_t size)
{
    t_byte* bytes = (t_byte*)array;

    for (size_t i = 0; i < size; i++)
    {
        bytes[i] = rand();
    }
}

char sign(int n)
{
    if (n > 0)
        return 1;
    if (n < 0)
        return -1;
    return 0;
}

size_t random_size()
{
    return rand() % MAX_SIZE;
}

int random_int()
{
    return rand() - RAND_MAX / 2;
}
