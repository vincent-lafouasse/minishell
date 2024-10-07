#include "gtest/gtest.h"

#include <string>
#include <vector>

extern "C"
{
#include "parse/tokenize/t_token.h"
#include "parse/tokenize/tokenize.h"
};

struct TokenList
{
    TokenList();
    TokenList(const std::string &src);
    TokenList(const std::vector<t_token> &src);
    ~TokenList();

    void assert_eq(const t_token_list* other);
    void assert_eq(const TokenList& other);

    t_token_list *head;
};

TokenList::TokenList() : head(nullptr) {}

TokenList::TokenList(const std::string &src) : head(tokenize(src.c_str())) {}

TokenList::TokenList(const std::vector<t_token> &src) : head(nullptr)
{
    for (const t_token &token : src)
    {
        tkl_push_back(&head, token);
    }
}

TokenList::~TokenList() { tkl_clear(&head); }

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

/*
TEST(Tokenize, SingleWord) {}
TEST(Tokenize, SeparatedWords) {}
TEST(Tokenize, SingleQuotedWord) {}
TEST(Tokenize, SeparatedQuotedWords) {}
TEST(Tokenize, SeparatedQuotedAndUnquotedWords) {}
TEST(Tokenize, JoinedQuotedAndUnquotedWords) {}
TEST(Tokenize, JoinedQuotedWords) {}
TEST(Tokenize, RejectsUnterminatedQuotes) {}
TEST(Tokenize, TabSeparatedTokens) {}
TEST(Tokenize, QuotesEscapeOperators) {}
TEST(Tokenize, LogicalOperatorsAndWords) {}
TEST(Tokenize, LogicalOperatorsAndWords) {}
TEST(Tokenize, WordsAndRedirectionOperators) {}
TEST(Tokenize, WordsAndLogicalOperators) {}
TEST(Tokenize, WordsAndControlOperators) {}
TEST(Tokenize, WordsAndParens) {}
TEST(Tokenize, JoinedWordsAndRedirectionOperators) {}
TEST(Tokenize, JoinedWordsAndLogicalOperators) {}
TEST(Tokenize, JoinedWordsAndControlOperators) {}
TEST(Tokenize, JoinedWordsAndParens) {}
TEST(Tokenize, AllJoinedOperatorsWithoutWords) {}
TEST(Tokenize, WordsAndOperatorsInNestedParens) {}
TEST(Tokenize, WordsInsideOfParens) {}
TEST(Tokenize, WordsAndControlPlusLogicalOperators) {}
TEST(Tokenize, WordsAndRedirectionOperators) {}
TEST(Tokenize, WordsAndAllOperators) {}
TEST(Tokenize, WordsAndAllOperatorsPlusParens) {}
TEST(Tokenize, SeparatorsYieldEmptyTokenList) {}
TEST(Tokenize, LogicalAndControlOperatorsWithWords) {}
TEST(Tokenize, Pipex) {}
TEST(Tokenize, BigPipex) {}
TEST(Tokenize, BigPipexNoWhitespace) {}
TEST(Tokenize, BigPipexLotsOfWhitespace) {}
*/


TEST(Tokenize, Pipex)
{
    const char *source = "< infile cmd1 | cmd2 > outfile";
    std::vector<t_token> expected = {
        Token(L_ANGLE_BRACKET, nullptr),
        Token(WORD, "infile"),
        Token(WORD, "cmd1"),
        Token(PIPE, nullptr),
        Token(WORD, "cmd2"),
        Token(R_ANGLE_BRACKET, nullptr),
        Token(WORD, "outfile"),
        Token(EOF_TOKEN, nullptr),
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
        Token(WORD, "file.cpp"), Token(PIPE, nullptr),
        Token(WORD, "cat"),      Token(PIPE, nullptr),
        Token(WORD, "sort"),     Token(PIPE, nullptr),
        Token(WORD, "wc"),       Token(PIPE, nullptr),
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
        Token(WORD, "file.cpp"), Token(PIPE, nullptr),
        Token(WORD, "cat"),      Token(PIPE, nullptr),
        Token(WORD, "sort"),     Token(PIPE, nullptr),
        Token(WORD, "wc"),       Token(PIPE, nullptr),
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
        Token(WORD, "file.cpp"), Token(PIPE, nullptr),
        Token(WORD, "cat"),      Token(PIPE, nullptr),
        Token(WORD, "sort"),     Token(PIPE, nullptr),
        Token(WORD, "wc"),       Token(PIPE, nullptr),
        Token(WORD, "a_cmd"),    Token(DR_ANGLE_BRACKET, nullptr),
        Token(WORD, "outfile"),  Token(EOF_TOKEN, nullptr),
    };

    t_token_list *tokens = tokenize(source);
    assert_tkl_equality(tokens, expected);
}
