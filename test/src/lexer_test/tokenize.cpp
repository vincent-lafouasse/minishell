#include "gtest/gtest.h"

#include <vector>

extern "C"
{
#include "tokenize/tokenize.h"
};

TEST(Tokenize, Simple)
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

TEST(Tokenize, Pipex)
{
    t_error err;

    const char *source = "< infile cmd1 | cmd2 > outfile";
    std::vector<t_token> expected = {
        (t_token){.type = L_ANGLE_BRACKET, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"infile"},
        (t_token){.type = WORD, .literal = (char *)"cmd1"},
        (t_token){.type = PIPE_TOKEN, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"cmd2"},
        (t_token){.type = R_ANGLE_BRACKET, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"outfile"},
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

TEST(Tokenize, BigPipex)
{
    t_error err;

    const char *source =
        "cppcheck < file.cpp | cat | sort | wc | a_cmd >> outfile";
    std::vector<t_token> expected = {
        (t_token){.type = WORD, .literal = (char *)"cppcheck"},
        (t_token){.type = L_ANGLE_BRACKET, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"file.cpp"},
        (t_token){.type = PIPE_TOKEN, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"cat"},
        (t_token){.type = PIPE_TOKEN, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"sort"},
        (t_token){.type = PIPE_TOKEN, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"wc"},
        (t_token){.type = PIPE_TOKEN, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"a_cmd"},
        (t_token){.type = DR_ANGLE_BRACKET, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"outfile"},
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

TEST(Tokenize, BigPipexNoWhitespace)
{
    t_error err;

    const char *source = "cppcheck<file.cpp|cat|sort|wc|a_cmd>>outfile";
    std::vector<t_token> expected = {
        (t_token){.type = WORD, .literal = (char *)"cppcheck"},
        (t_token){.type = L_ANGLE_BRACKET, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"file.cpp"},
        (t_token){.type = PIPE_TOKEN, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"cat"},
        (t_token){.type = PIPE_TOKEN, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"sort"},
        (t_token){.type = PIPE_TOKEN, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"wc"},
        (t_token){.type = PIPE_TOKEN, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"a_cmd"},
        (t_token){.type = DR_ANGLE_BRACKET, .literal = NULL},
        (t_token){.type = WORD, .literal = (char *)"outfile"},
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
