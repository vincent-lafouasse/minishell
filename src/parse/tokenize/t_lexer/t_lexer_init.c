#include "libft/string.h"
#include "t_lexer.h"

t_lexer lexer_init(const char *source)
{
    return ((t_lexer){.source = source, .src_len = ft_strlen(source)});
}
