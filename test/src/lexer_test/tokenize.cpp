#include "gtest/gtest.h"

#include <vector>

extern "C"
{
#include "tokenize/tokenize.h"
};

static void assert_tkl_equality(const t_token_list *tokens,
                                const std::vector<t_token> &expected)
{
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

static t_token Token(t_token_type type, const char *literal)
{
    return (t_token){.type = type, .literal = (char *)literal};
}

TEST(Tokenize, Simple)
{
    const char *source = "hello > world";
    std::vector<t_token> expected = {
        Token(WORD, "hello"),
        Token(R_ANGLE_BRACKET, NULL),
        Token(WORD, "world"),
        Token(EOF_TOKEN, NULL),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, Pipex)
{
    const char *source = "< infile cmd1 | cmd2 > outfile";
    std::vector<t_token> expected = {
        Token(L_ANGLE_BRACKET, NULL), Token(WORD, "infile"),
        Token(WORD, "cmd1"),          Token(PIPE_TOKEN, NULL),
        Token(WORD, "cmd2"),          Token(R_ANGLE_BRACKET, NULL),
        Token(WORD, "outfile"),       Token(EOF_TOKEN, NULL),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, BigPipex)
{
    const char *source =
        "cppcheck < file.cpp | cat | sort | wc | a_cmd >> outfile";
    std::vector<t_token> expected = {
        Token(WORD, "cppcheck"), Token(L_ANGLE_BRACKET, NULL),
        Token(WORD, "file.cpp"), Token(PIPE_TOKEN, NULL),
        Token(WORD, "cat"),      Token(PIPE_TOKEN, NULL),
        Token(WORD, "sort"),     Token(PIPE_TOKEN, NULL),
        Token(WORD, "wc"),       Token(PIPE_TOKEN, NULL),
        Token(WORD, "a_cmd"),    Token(DR_ANGLE_BRACKET, NULL),
        Token(WORD, "outfile"),  Token(EOF_TOKEN, NULL),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, BigPipexNoWhitespace)
{
    const char *source = "cppcheck<file.cpp|cat|sort|wc|a_cmd>>outfile";
    std::vector<t_token> expected = {
        Token(WORD, "cppcheck"), Token(L_ANGLE_BRACKET, NULL),
        Token(WORD, "file.cpp"), Token(PIPE_TOKEN, NULL),
        Token(WORD, "cat"),      Token(PIPE_TOKEN, NULL),
        Token(WORD, "sort"),     Token(PIPE_TOKEN, NULL),
        Token(WORD, "wc"),       Token(PIPE_TOKEN, NULL),
        Token(WORD, "a_cmd"),    Token(DR_ANGLE_BRACKET, NULL),
        Token(WORD, "outfile"),  Token(EOF_TOKEN, NULL),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, BigPipexLotsOfWhitespace)
{
    const char *source = "   cppcheck   <   file.cpp   |   cat | sort | wc | "
                         "a_cmd >> outfile   ";
    std::vector<t_token> expected = {
        Token(WORD, "cppcheck"), Token(L_ANGLE_BRACKET, NULL),
        Token(WORD, "file.cpp"), Token(PIPE_TOKEN, NULL),
        Token(WORD, "cat"),      Token(PIPE_TOKEN, NULL),
        Token(WORD, "sort"),     Token(PIPE_TOKEN, NULL),
        Token(WORD, "wc"),       Token(PIPE_TOKEN, NULL),
        Token(WORD, "a_cmd"),    Token(DR_ANGLE_BRACKET, NULL),
        Token(WORD, "outfile"),  Token(EOF_TOKEN, NULL),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}
