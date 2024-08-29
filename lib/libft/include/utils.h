#pragma once

#include <stddef.h>
#include "libft.h"

void scramble_array(void* array, size_t size);
void compare_bytes(t_byte*, t_byte*, size_t size);
void compare_strings(const char* s1, const char* s2);

char sign(int n);

size_t random_size(void);
int random_int(void);
