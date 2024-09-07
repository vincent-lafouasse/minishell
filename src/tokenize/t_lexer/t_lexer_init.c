#include "t_lexer.h"

static size_t ft_strlen(const char *s);

t_lexer lexer_init(const char *source)
{
    return ((t_lexer){.source = source, .src_len = ft_strlen(source)});
}

static size_t ft_strlen(const char *s)
{
    size_t sz;

    sz = 0;
    while (s[sz])
        sz++;
    return (sz);
}
