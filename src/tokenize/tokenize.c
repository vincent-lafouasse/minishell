#include "tokenize.h"
#include "libft/string.h"
#include "log/log.h"
#include "t_lexer/t_lexer.h"
#include <stdio.h>

#define WHITESPACE " "

static void skip_whitespace(t_lexer *lexer);
static void cleanup_log(t_lexer *lexer, t_error err);

/*
 * TAKE OOM SERIOUSLY IN PRODUCTION MODE
 * ie do not ignore failability of tkl_push_back
 */

t_token_list *tokenize(const char *input)
{
    t_lexer lexer;
    t_token token;
    t_error err;

    lexer = lexer_init(input);
    skip_whitespace(&lexer);
    while (lexer.current < lexer.src_len)
    {
        lexer.start = lexer.current;
        err = lexer_scan_next_token(&lexer, &token);
        if (err != NO_ERROR)
            return cleanup_log(&lexer, err), NULL;
        err = tkl_push_back(&lexer.tokens, token);
        skip_whitespace(&lexer);
    }
    tkl_push_back(&lexer.tokens, (t_token){.type = EOF_TOKEN});
    return (lexer.tokens);
}

static void skip_whitespace(t_lexer *lexer)
{
    while (lexer->current < lexer->src_len &&
           ft_strchr(WHITESPACE, lexer_peek(lexer)) != NULL)
        lexer_advance(lexer);
}

static void cleanup_log(t_lexer *lexer, t_error err)
{
    log_token_list(lexer->tokens);
    log_error(err);
    tkl_clear(&lexer->tokens);
}
