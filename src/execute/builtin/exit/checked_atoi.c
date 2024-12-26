#include <stdbool.h>
#include <stdint.h>

static bool			parse_valid_number(int8_t sign, const char *s, int32_t *out);
static bool    	   str_is_all_zeros(const char *num);
static uint32_t	   ft_abs(int32_t n);

bool    checked_atou(const char *s, uint32_t *out);

bool    checked_atoi(const char *s, int32_t *out)
{
	int8_t    sign;

	sign = 1;
	while (*s == ' ')
		s++;
	if (*s == '-')
	{
		sign = -1;
		s++;
	}
	else if (*s == '+')
		s++;
	if (str_is_all_zeros(s))
	{
		*out = 0;
		return (0);
	}
	while (*s == '0')
		s++;
	return (parse_valid_number(sign, s, out));
}

static bool parse_valid_number(int8_t sign, const char *s, int32_t *out)
{
	uint32_t    absolute_value;

	if (!checked_atou(s, &absolute_value))
		return (false);
	if (sign == 1 && absolute_value > INT32_MAX)
		return (false);
	if (sign == -1 && absolute_value > ft_abs(INT32_MIN))
		return (false);
	*out = sign * (int32_t)absolute_value;
	return (true);
}

static bool		str_is_all_zeros(const char *s)
{
	if (!s || *s == '\0')
		return (false);
	while (*s)
	{
		if (*s != '0')
			return (false);
		s++;
	}
	return (true);
}

static uint32_t    ft_abs(int32_t n)
{
	if (n < 0)
		return -n;
	return n;
}
