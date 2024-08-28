#include "libft.h"
#include "unity.h"

#include <stdio.h>

void setUp(void) {}

void tearDown(void) {}

void run_test_strlen(void);
void run_test_ctype(void);
void run_test_atoi(void);
void run_test_memset(void);
void run_test_bzero(void);
void run_test_strchr(void);
void run_test_calloc(void);
void run_test_strlcpy(void);
void run_test_strdup(void);
void run_test_memcpy(void);
void run_test_memcmp(void);
void run_test_strncmp(void);
void run_test_strlcat(void);

int main(void)
{
    UnityBegin("Libft");
    run_test_strlen();
    run_test_ctype();
    run_test_atoi();
    run_test_memset();
    run_test_bzero();
    run_test_calloc();
    run_test_strchr();
    run_test_strlcpy();
    run_test_strlcat();
    run_test_strdup();
    run_test_memcpy();
    run_test_memcmp();
    run_test_strncmp();
    return (UnityEnd());
}
