#include "tokenize.h"
#include "t_lexer.h"
#include <stddef.h>

t_token_list *tokenize(const char *input)
{
    t_lexer lexer;

    lexer = lexer_init(input);
    (void)lexer;
    return NULL;
}
