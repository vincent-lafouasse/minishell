#include "gtest/gtest.h"

#include <vector>

extern "C"
{
#include "tokenize/t_lexer/lexer_internals.h"
#include "tokenize/t_lexer/t_lexer.h"
#include "tokenize/tokenize.h"
};

TEST(Lexer, TokenizeSimple)
{
    t_error err;

    const char *source = "hello > world";
    std::vector<t_token> expected = {
        (t_token){.type = WORD, .literal = (char *)"hello"},
        (t_token){.type = R_ANGLE_BRACKET, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"world"},
        (t_token){.type = EOF_TOKEN, .literal = nullptr},
    };

    t_token_list *tokens = tokenize(source);
    ASSERT_NE(tokens, nullptr);

    auto expected_it = expected.cbegin();

    while (expected_it != expected.cend() && tokens)
    {
        ASSERT_EQ(expected_it->type, tokens->token.type);
        ASSERT_STREQ(expected_it->literal, tokens->token.literal);
        expected_it++;
        tokens = tokens->next;
    }
    ASSERT_EQ(tokens, nullptr);
    ASSERT_EQ(expected_it, expected.cend());
}
