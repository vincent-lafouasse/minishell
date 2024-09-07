#include "t_lexer.h"
#include "libft/string.h"

t_lexer lexer_init(const char *source)
{
    return ((t_lexer){.source = source, .src_len = ft_strlen(source)});
}
