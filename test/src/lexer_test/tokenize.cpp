#include "gtest/gtest.h"

#include <vector>

extern "C"
{
#include "tokenize/t_token.h"
#include "tokenize/tokenize.h"
};

static void assert_tkl_equality(const t_token_list *tokens,
                                const std::vector<t_token> &expected_tokens)
{
    auto expected_it = expected_tokens.cbegin();

    while (expected_it != expected_tokens.cend() && tokens)
    {
        const t_token expected = *expected_it;
        const t_token actual = tokens->token;

        ASSERT_EQ(expected.type, actual.type)
            << "Mismatched token types, expected " << token_repr(expected)
            << " was " << token_repr(actual);
        ASSERT_STREQ(expected.literal, actual.literal);
        expected_it++;
        tokens = tokens->next;
    }
    ASSERT_EQ(tokens, nullptr);
    ASSERT_EQ(expected_it, expected_tokens.cend());
}

static t_token Token(t_token_type type, const char *literal)
{
    return (t_token){.type = type, .literal = const_cast<char *>(literal)};
}

TEST(Tokenize, Word)
{
    const char *source = "hello";
    std::vector<t_token> expected = {
        Token(WORD, "hello"),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, SingleQuotedString)
{
    const char *source = "'hello'";
    std::vector<t_token> expected = {
        Token(SINGLE_QUOTE_STRING, "hello"),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, DoubleQuotedString)
{
    const char *source = "\"hello\"";
    std::vector<t_token> expected = {
        Token(DOUBLE_QUOTE_STRING, "hello"),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, WordFollowedByUnterminatedStringIsInvalid)
{
    const char *source = "hello \"world";

    t_token_list *tokens = tokenize(source);
    ASSERT_EQ(tokens, nullptr);
}

TEST(Tokenize, WhitespaceSeperatedWords)
{
    const char *source = "hello world";
    std::vector<t_token> expected = {
        Token(WORD, "hello"),
        Token(WORD, "world"),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, WhitespaceSeperatedWordAndString)
{
    const char *source = "hello 'world'";
    std::vector<t_token> expected = {
        Token(WORD, "hello"),
        Token(SINGLE_QUOTE_STRING, "world"),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, TabSeperatedWords)
{
    const char *source = "hello\tworld";
    std::vector<t_token> expected = {
        Token(WORD, "hello"),
        Token(WORD, "world"),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, Whitespaces)
{
    const char *source = "   ";
    std::vector<t_token> expected = {
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, Tabs)
{
    const char *source = "\t\t\t";
    std::vector<t_token> expected = {
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, Operator)
{
    const char *source = "<";
    std::vector<t_token> expected = {
        Token(L_ANGLE_BRACKET, nullptr),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, SingleQuotedOperator)
{
    const char *source = "'<'";
    std::vector<t_token> expected = {
        Token(SINGLE_QUOTE_STRING, "<"),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, ConcatenatedWordsAndStrings)
{
    const char *source = "e\"c\"h'o'";
    std::vector<t_token> expected = {
        Token(WORD, "e"),
        Token(DOUBLE_QUOTE_STRING, "c"),
        Token(WORD, "h"),
        Token(SINGLE_QUOTE_STRING, "o"),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, ConcatenatedSentence)
{
    const char *source = "e\"c\"h'o' hello \"world\"";
    std::vector<t_token> expected = {
        Token(WORD, "e"),
        Token(DOUBLE_QUOTE_STRING, "c"),
        Token(WORD, "h"),
        Token(SINGLE_QUOTE_STRING, "o"),
        Token(WORD, "hello"),
        Token(DOUBLE_QUOTE_STRING, "world"),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, SimpleCommandWithOutputRedirection)
{
    const char *source = "hello > world";
    std::vector<t_token> expected = {
        Token(WORD, "hello"),
        Token(R_ANGLE_BRACKET, nullptr),
        Token(WORD, "world"),
        Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, Pipex)
{
    const char *source = "< infile cmd1 | cmd2 > outfile";
    std::vector<t_token> expected = {
        Token(L_ANGLE_BRACKET, nullptr), Token(WORD, "infile"),
        Token(WORD, "cmd1"),             Token(PIPE_TOKEN, nullptr),
        Token(WORD, "cmd2"),             Token(R_ANGLE_BRACKET, nullptr),
        Token(WORD, "outfile"),          Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, BigPipex)
{
    const char *source =
        "cppcheck < file.cpp | cat | sort | wc | a_cmd >> outfile";
    std::vector<t_token> expected = {
        Token(WORD, "cppcheck"), Token(L_ANGLE_BRACKET, nullptr),
        Token(WORD, "file.cpp"), Token(PIPE_TOKEN, nullptr),
        Token(WORD, "cat"),      Token(PIPE_TOKEN, nullptr),
        Token(WORD, "sort"),     Token(PIPE_TOKEN, nullptr),
        Token(WORD, "wc"),       Token(PIPE_TOKEN, nullptr),
        Token(WORD, "a_cmd"),    Token(DR_ANGLE_BRACKET, nullptr),
        Token(WORD, "outfile"),  Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, BigPipexNoWhitespace)
{
    const char *source = "cppcheck<file.cpp|cat|sort|wc|a_cmd>>outfile";
    std::vector<t_token> expected = {
        Token(WORD, "cppcheck"), Token(L_ANGLE_BRACKET, nullptr),
        Token(WORD, "file.cpp"), Token(PIPE_TOKEN, nullptr),
        Token(WORD, "cat"),      Token(PIPE_TOKEN, nullptr),
        Token(WORD, "sort"),     Token(PIPE_TOKEN, nullptr),
        Token(WORD, "wc"),       Token(PIPE_TOKEN, nullptr),
        Token(WORD, "a_cmd"),    Token(DR_ANGLE_BRACKET, nullptr),
        Token(WORD, "outfile"),  Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}

TEST(Tokenize, BigPipexLotsOfWhitespace)
{
    const char *source = "   cppcheck   <   file.cpp   |   cat | sort | wc | "
                         "a_cmd >> outfile   ";
    std::vector<t_token> expected = {
        Token(WORD, "cppcheck"), Token(L_ANGLE_BRACKET, nullptr),
        Token(WORD, "file.cpp"), Token(PIPE_TOKEN, nullptr),
        Token(WORD, "cat"),      Token(PIPE_TOKEN, nullptr),
        Token(WORD, "sort"),     Token(PIPE_TOKEN, nullptr),
        Token(WORD, "wc"),       Token(PIPE_TOKEN, nullptr),
        Token(WORD, "a_cmd"),    Token(DR_ANGLE_BRACKET, nullptr),
        Token(WORD, "outfile"),  Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}
