#include "tokenize.h"
#include "t_lexer/t_lexer.h"
#include "log/log.h"

static void cleanup_log(t_lexer *lexer, t_error err);

/*
 * TAKE OOM SERIOUSLY IN PRODUCTION MODE
 * ie do not ignore failability of tkl_push_back
 */

t_error tokenize(const char *input, t_token_list** out)
{
    t_lexer lexer;
    t_token token;
    t_error err;

    lexer = lexer_init(input);
    lexer_skip_whitespace(&lexer);
    while (lexer.current < lexer.src_len)
    {
        lexer.start = lexer.current;
        err = lexer_scan_next_token(&lexer, &token);
        if (err != NO_ERROR)
        {
            cleanup_log(&lexer, err);
            return (err);
        }
        err = tkl_push_back(&lexer.tokens, token);
        lexer_skip_whitespace(&lexer);
    }
    tkl_push_back(&lexer.tokens, (t_token){.type = EOF_TOKEN});
    *out = lexer.tokens;
    return (NO_ERROR);
}

static void cleanup_log(t_lexer *lexer, t_error err)
{
    log_token_list(lexer->tokens);
    log_error(err);
    tkl_clear(&lexer->tokens);
}
