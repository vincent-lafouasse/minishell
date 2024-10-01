#include "parse/t_symbol.h"
#include "gtest/gtest.h"

#include <cstring>
#include <vector>

extern "C"
{
#include "log/log.h"
#include "parse/parse.h"
#include "tokenize/t_token_list/t_token_list.h"
};

/*
"hello"
"hello world"
"bat cat dash idk im just saying stuff"
"vim > emacs"
"hello | world | my guy"
"hello && world << blegh || my > haha | guy || < lol (idek)"
*/

t_token Token(t_token_type type)
{
    return (t_token){.type = type, .literal = nullptr};
}

t_token Token(const char *word)
{
    return (t_token){.type = WORD, .literal = (char *)word};
}

struct Tokens
{
    t_token_list *self;

    Tokens(const std::vector<t_token> &tokens) : self{nullptr}
    {
        for (const t_token &token : tokens)
        {
            char *literal =
                token.literal != nullptr ? strdup(token.literal) : nullptr;
            t_token element = (t_token){.type = token.type, .literal = literal};

            tkl_push_back(&self, element);
        }
    }

    ~Tokens() { tkl_clear(&self); }
};
void assert_successful_parsing(const Tokens &tokens)
{
    t_symbol sym;
    t_command parsed;
    t_error err = parse_command(tokens.self, &sym);

    ASSERT_EQ(err, NO_ERROR);
}

TEST(Parser, Pipex)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(R_ANGLE_BRACKET),
        Token("world"),
        Token(PIPE_TOKEN),
        Token("goodbye"),
        Token(L_ANGLE_BRACKET),
        Token("mars"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}
