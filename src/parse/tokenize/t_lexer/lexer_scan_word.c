#include "error/t_error.h"
#include "lexer_internals.h"

#include "libft/ft_string.h"
#include "libft/string.h"
#include "libft/ctype.h"

#include <assert.h> // bad
#include <stdlib.h>

#define SIMPLE_WORD_BREAK SHELL_BREAK_CHARS "\"'"

t_error extract_quoted_string(t_lexer* lexer, char** str_out);
t_error extract_simple_word(t_lexer* lexer, char** str_out);
t_error append_next_word_or_string(t_lexer* lexer, char **onto);

t_error lexer_scan_word(t_lexer *lexer, t_token *out)
{
    char* str_out = ft_strdup("");
    bool must_continue = true;
    t_error err;

    while (must_continue)
    {
        err = append_next_word_or_string(lexer, &str_out);
        if (err != NO_ERROR)
        {
            free(str_out);
            return err;
        }
        must_continue = ft_strchr(SHELL_BREAK_CHARS, lexer_peek(lexer)) == NULL;
    }
    return fill_token((t_token){.type = WORD, .literal = str_out}, out);
}

t_error append_next_word_or_string(t_lexer* lexer, char **onto)
{
    char* next;
    char* buffer;
    t_error err;

    if (ft_strchr("\"'", lexer_peek(lexer)))
    {
        err = extract_quoted_string(lexer, &next);
        if (err != NO_ERROR)
            return err;
        buffer = ft_strjoin(*onto, next);
        if (!buffer)
        {
            free(next);
            return E_OOM;
        }
    }
    else
{
        err = extract_simple_word(lexer, &next);
        if (err != NO_ERROR)
            return err;
        buffer = ft_strjoin(*onto, next);
        if (!buffer)
        {
            free(next);
            return E_OOM;
        }
    }
    free(*onto);
    *onto = buffer;
    return NO_ERROR;
}

t_error extract_simple_word(t_lexer* lexer, char** str_out)
{
    size_t start = lexer->current;

    while (ft_strchr(SIMPLE_WORD_BREAK, lexer_peek(lexer)) == NULL &&
           lexer->current < lexer->src_len)
        lexer_advance(lexer);
    
    size_t sz = lexer->current - start;
    char* out = ft_substr(lexer->source, start, sz);
    if (out == NULL)
        return E_OOM;
    *str_out = out;
    return NO_ERROR;
}

t_error extract_quoted_string(t_lexer* lexer, char** str_out)
{
    size_t start = lexer->current;
    char stop = lexer_advance(lexer);
    assert(stop == '"' || stop == '\'');

    while (lexer_peek(lexer) != stop && lexer->current < lexer->src_len)
        lexer_advance(lexer);
    if (lexer->current >= lexer->src_len)
        return E_UNTERMINATED_QUOTE;
    lexer_advance(lexer);

    size_t sz = lexer->current - start;
    char* out =  ft_substr(lexer->source, start, sz);
    if (out == NULL)
        return E_OOM;
    *str_out = out;
    return NO_ERROR;
}


char* easy_concatenation(char* a, char* b)
{
    char* out;

    out = ft_strjoin(a, b);
    free(a);
    free(b);
    return out;
}
