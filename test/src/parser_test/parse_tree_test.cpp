#include "gtest/gtest.h"

#include <cstring>
#include <vector>

extern "C"
{
#include "log/log.h"
#include "parse/parse.h"
#include "parse/t_symbol/t_symbol.h"
#include "parse/tokenize/t_token_list/t_token_list.h"
#include "error/t_error.h"

t_error	generate_parse_tree(const t_token_list *tokens, t_symbol *out);
};

#define FROM_FILE L_ANGLE_BRACKET
#define INTO_FILE R_ANGLE_BRACKET
#define APPEND_INTO_FILE DR_ANGLE_BRACKET
#define HERE_DOCUMENT DL_ANGLE_BRACKET

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

    ~Tokens() { tkl_clear(&self, free); }
};
void assert_unexpected_token_during_parsing(const Tokens &tokens)
{
    t_symbol sym;
    t_command parsed;
    t_error err = generate_parse_tree(tokens.self, &sym);

    if (err == E_UNEXPECTED_TOKEN)
    {
        SUCCEED();
        return ;
    }

    log_token_list(tokens.self);
    if (err == NO_ERROR)
        symbol_clear(sym);

    ASSERT_EQ(err, E_UNEXPECTED_TOKEN) << error_repr(err);
}
void assert_successful_parsing(const Tokens &tokens)
{
    t_symbol sym;
    t_command parsed;
    t_error err = generate_parse_tree(tokens.self, &sym);

    if (err != NO_ERROR)
    {
        log_token_list(tokens.self);
        ASSERT_EQ(err, NO_ERROR) << error_repr(err);
    }
    else
    {
        symbol_clear(sym);
        SUCCEED();
    }
}

/*
TEST(ParseTree, NoCommand)
{
    Tokens tokens = Tokens({
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}
*/

TEST(ParseTree, SingleWord)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, ManyWords)
{
    Tokens tokens = Tokens({
        Token("word"),
        Token("hello"),
        Token("word"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, SingleRedirection)
{
    Tokens tokens = Tokens({
        Token(FROM_FILE),
        Token("input"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, ManyRedirections)
{
    Tokens tokens = Tokens({
        Token(FROM_FILE),
        Token("input"),
        Token(INTO_FILE),
        Token("output"),
        Token(FROM_FILE),
        Token("input_again"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, WordAndSingleRedirection)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(FROM_FILE),
        Token("input"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, WordAndManyTrailingRedirections)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(INTO_FILE),
        Token("output"),
        Token(FROM_FILE),
        Token("input"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, WordAndManySameTrailingRedirections)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(INTO_FILE),
        Token("output"),
        Token(INTO_FILE),
        Token("output_two"),
        Token(INTO_FILE),
        Token("output_three"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, ManyMixedRedirectionsAndWords)
{
    Tokens tokens = Tokens({
        Token(FROM_FILE),
        Token("input"),
        Token("hello"),
        Token(INTO_FILE),
        Token("output"),
        Token("world"),
        Token(INTO_FILE),
        Token("output_two"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, SimplePipeline)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(PIPE),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, SimplePipelineWithManyWords)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, MultiPipelineWithSingleWords)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, PipelineWithOnlyRedirections)
{
    Tokens tokens = Tokens({
        Token(FROM_FILE),
        Token("output_two"),
        Token(PIPE),
        Token(INTO_FILE),
        Token("output_two"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, SimplePipelineWithLeadingRedirection)
{
    Tokens tokens = Tokens({
        Token(INTO_FILE),
        Token("output_two"),
        Token("hello"),
        Token(PIPE),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, PipexWithMixedWordsAndRedirections)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(FROM_FILE),
        Token("infile"),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(INTO_FILE),
        Token("mars"),
        Token("hello"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, Pipex)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(FROM_FILE),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(INTO_FILE),
        Token("mars"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, SimpleSubshell)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token("mars"),
        Token(R_PAREN),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, SubshellWithTrailingRedirection)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token("mars"),
        Token(R_PAREN),
        Token(INTO_FILE),
        Token("mars"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, SubshellWithPipexWithMixedWordsAndRedirections)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token(FROM_FILE),
        Token("infile"),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(INTO_FILE),
        Token("mars"),
        Token("hello"),
        Token(R_PAREN),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, PipelineInSubshellWithTrailingRedirection)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(R_PAREN),
        Token(INTO_FILE),
        Token("mars"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, SubshellWithManyTrailingRedirections)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(R_PAREN),
        Token(INTO_FILE),
        Token("infile"),
        Token(FROM_FILE),
        Token("outfile"),
        Token(FROM_FILE),
        Token("infile2"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, WordsSeperatedByConditional)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token("world"),
        Token(OR_OR),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, WordsAndSubshellSeperatedByConditional)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token("world"),
        Token(OR_OR),
        Token(L_PAREN),
        Token("dlrow"),
        Token(PIPE),
        Token("olleh"),
        Token(R_PAREN),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, PipelinesSeperatedByConditonal)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(OR_OR),
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, PipelinesSeperatedByConditonalWithPriorities)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(OR_OR),
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(R_PAREN),
        Token(PIPE),
        Token("goodbye"),
        Token(AND_AND),
        Token(L_PAREN),
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(R_PAREN),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, NestedSimpleSubshell)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token(L_PAREN),
        Token(L_PAREN),
        Token(L_PAREN),
        Token(L_PAREN),
        Token("hello"),
        Token("world"),
        Token(R_PAREN),
        Token(R_PAREN),
        Token(R_PAREN),
        Token(R_PAREN),
        Token(R_PAREN),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, Wtf)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("bat"),
        Token("cat"),
        Token(R_PAREN),
        Token(HERE_DOCUMENT),
        Token("bash"),
        Token(OR_OR),
        Token(L_PAREN),
        Token("sh"),
        Token(AND_AND),
        Token(L_PAREN),
        Token("dash"),
        Token(R_PAREN),
        Token(R_PAREN),
        Token(PIPE),
        Token(L_PAREN),
        Token("ulimit"),
        Token(R_PAREN),
        Token(AND_AND),
        Token("idk"),
        Token(HERE_DOCUMENT),
        Token("im"),
        Token("just"),
        Token(INTO_FILE),
        Token("saying"),
        Token(PIPE),
        Token("stuff"),
        Token(EOF_TOKEN),
    });

    assert_successful_parsing(tokens);
}

TEST(ParseTree, RejectsTrailingBracket)
{
    Tokens tokens = Tokens({
        Token("abc"),
        Token(AND_AND),
        Token("hello"),
        Token(FROM_FILE),
        Token(PIPE),
        Token("world"),
        Token(EOF_TOKEN),
    });

    assert_unexpected_token_during_parsing(tokens);
}

TEST(ParseTree, RejectsSubshellWithLeadingRedirections)
{
    Tokens tokens = Tokens({
        Token(FROM_FILE),
        Token("infile"),
        Token(INTO_FILE),
        Token("mars"),
        Token(L_PAREN),
        Token("hello"),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(R_PAREN),
        Token(EOF_TOKEN),
    });

    assert_unexpected_token_during_parsing(tokens);
}

TEST(ParseTree, RejectsNoMatchingSubshellParen)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(PIPE),
        Token(L_PAREN),
        Token(R_PAREN),
        Token(EOF_TOKEN),
    });

    assert_unexpected_token_during_parsing(tokens);
}

TEST(ParseTree, RejectsUnterminatedPipeline)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(PIPE),
        Token(EOF_TOKEN),
    });

    assert_unexpected_token_during_parsing(tokens);
}

TEST(ParseTree, RejectsUnterminatedSubshell)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(EOF_TOKEN),
    });

    assert_unexpected_token_during_parsing(tokens);
}

TEST(ParseTree, RejectsRedirectionWithNoFollowingWord)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token(APPEND_INTO_FILE),
        Token(INTO_FILE),
        Token("world"),
        Token(EOF_TOKEN),
    });

    assert_unexpected_token_during_parsing(tokens);
}

TEST(ParseTree, RejectsEmptySubshell)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token(R_PAREN),
        Token(EOF_TOKEN),
    });

    assert_unexpected_token_during_parsing(tokens);
}

TEST(ParseTree, RejectsEmptySubshellWithTrailingRedirection)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token(R_PAREN),
        Token(INTO_FILE),
        Token("file"),
        Token(EOF_TOKEN),
    });

    assert_unexpected_token_during_parsing(tokens);
}

TEST(ParseTree, RejectsWtfWithLeadingRedirectedSubshell)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("bat"),
        Token("cat"),
        Token(R_PAREN),
        Token(HERE_DOCUMENT),
        Token("bash"),
        Token(OR_OR),
        Token(L_PAREN),
        Token("sh"),
        Token(AND_AND),
        Token(L_PAREN),
        Token("dash"),
        Token(R_PAREN),
        Token(R_PAREN),
        Token(PIPE),
        Token(INTO_FILE),
        Token("woops"),
        Token(L_PAREN),
        Token("ulimit"),
        Token(R_PAREN),
        Token(AND_AND),
        Token("idk"),
        Token(HERE_DOCUMENT),
        Token("im"),
        Token("just"),
        Token(INTO_FILE),
        Token("saying"),
        Token(PIPE),
        Token("stuff"),
        Token(EOF_TOKEN),
    });

    assert_unexpected_token_during_parsing(tokens);
}
