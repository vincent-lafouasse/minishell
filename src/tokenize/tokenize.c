#include "tokenize.h"
#include "libft/string.h"
#include "log/log.h"
#include "t_lexer.h"
#include <stdio.h>

#define WHITESPACE " "

static void skip_whitespace(t_lexer *lexer);
static void cleanup_exit(t_lexer *lexer, t_error err);

t_token_list *tokenize(const char *input)
{
    t_lexer lexer;
    t_token token;
    t_error err;

    lexer = lexer_init(input);
    while (lexer.current < lexer.src_len)
    {
        skip_whitespace(&lexer);
        lexer.start = lexer.current;
        err = lexer_scan_next_token(&lexer, &token);
        if (err != NO_ERROR)
            return cleanup_exit(&lexer, err), NULL;
        err = tkl_push_back(&lexer.tokens, token);
        if (err != NO_ERROR)
            return cleanup_exit(&lexer, err), NULL;
    }
    tkl_push_back(&lexer.tokens, (t_token){.type = EOF_TOKEN});
    if (err != NO_ERROR)
        return cleanup_exit(&lexer, err), NULL;
    log_token_list(lexer.tokens);
    return (lexer.tokens);
}

static void skip_whitespace(t_lexer *lexer)
{
    while (lexer->current < lexer->src_len &&
           ft_strchr(WHITESPACE, lexer_peek(lexer)) != NULL)
        lexer_advance(lexer);
}

static void cleanup_exit(t_lexer *lexer, t_error err)
{
    log_token_list(lexer->tokens);
    log_error(err);
    tkl_clear(&lexer->tokens);
}
