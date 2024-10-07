#pragma once

#include "t_lexer.h"

#define METACHARACTERS "()<>|&"
#define WHITESPACE " \t\n"
#define SHELL_BREAK_CHARS METACHARACTERS WHITESPACE

t_error lexer_scan_metacharacter_token(t_lexer *lexer, t_token *out);
t_error lexer_scan_string(t_lexer *lexer, t_token *out);
t_error lexer_scan_word(t_lexer *lexer, t_token *out);

char lexer_advance(t_lexer *lexer);
char lexer_peek(t_lexer *lexer);

t_error fill_token(t_token token, t_token *out);
