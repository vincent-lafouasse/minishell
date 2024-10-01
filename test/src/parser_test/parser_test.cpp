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

    ~Tokens() { tkl_clear(&self); }
};
void assert_unexpected_token_during_parsing(const Tokens &tokens)
{
    t_symbol sym;
    t_command parsed;
    t_error err = parse_command(tokens.self, &sym);

    ASSERT_EQ(err, E_UNEXPECTED_TOKEN);
}
void assert_successful_parsing(const Tokens &tokens)
{
    t_symbol sym;
    t_command parsed;
    t_error err = parse_command(tokens.self, &sym);

    ASSERT_EQ(err, NO_ERROR);
}

TEST(Parser, NoCommand)
{
    Tokens tokens = Tokens({
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, SingleWord)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, ManyWords)
{
    Tokens tokens = Tokens({
        Token("word"),
        Token("hello"),
        Token("word"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, SingleRedirection)
{
    Tokens tokens = Tokens({
        Token(FROM_FILE),
        Token("input"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, ManyRedirections)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, WordAndSingleRedirection)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(FROM_FILE),
        Token("input"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, WordAndManyTrailingRedirections)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(INTO_FILE),
        Token("output"),
        Token(FROM_FILE),
        Token("input"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, WordAndManySameTrailingRedirections)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, ManyMixedRedirectionsAndWords)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, SimplePipeline)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(PIPE),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, SimplePipelineWithManyWords)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, MultiPipelineWithSingleWords)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(PIPE),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, PipelineWithOnlyRedirections)
{
    Tokens tokens = Tokens({
        Token(FROM_FILE),
        Token("output_two"),
        Token(PIPE),
        Token(INTO_FILE),
        Token("output_two"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, SimplePipelineWithLeadingRedirection)
{
    Tokens tokens = Tokens({
        Token(INTO_FILE),
        Token("output_two"),
        Token("hello"),
        Token(PIPE),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, PipexWithMixedWordsAndRedirections)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, Pipex)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, SimpleSubshell)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token("mars"),
        Token(R_PAREN),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, SubshellWithTrailingRedirection)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, SubshellWithPipexWithMixedWordsAndRedirections)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, PipelineInSubshellWithTrailingRedirection)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, SubshellWithManyTrailingRedirections)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, WordsSeperatedByConditional)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token("world"),
        Token(OR_OR),
        Token("goodbye"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, WordsAndSubshellSeperatedByConditional)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, PipelinesSeperatedByConditonal)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, PipelinesSeperatedByConditonalWithPriorities)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, NestedSimpleSubshell)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, Wtf)
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
    log_token_list(tokens.self);

    assert_successful_parsing(tokens);
}

TEST(Parser, RejectsTrailingBracket)
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
    log_token_list(tokens.self);

    assert_unexpected_token_during_parsing(tokens);
}

TEST(Parser, RejectsSubshellWithLeadingRedirections)
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
    log_token_list(tokens.self);

    assert_unexpected_token_during_parsing(tokens);
}

TEST(Parser, RejectsNoMatchingSubshellParen)
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
    log_token_list(tokens.self);

    assert_unexpected_token_during_parsing(tokens);
}

TEST(Parser, RejectsUnterminatedPipeline)
{
    Tokens tokens = Tokens({
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(PIPE),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_unexpected_token_during_parsing(tokens);
}

TEST(Parser, RejectsUnterminatedSubshell)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token(PIPE),
        Token("world"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_unexpected_token_during_parsing(tokens);
}

TEST(Parser, RejectsRedirectionWithNoFollowingWord)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token("hello"),
        Token(APPEND_INTO_FILE),
        Token(INTO_FILE),
        Token("world"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_unexpected_token_during_parsing(tokens);
}

TEST(Parser, RejectsEmptySubshell)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token(R_PAREN),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_unexpected_token_during_parsing(tokens);
}

TEST(Parser, RejectsEmptySubshellWithTrailingRedirection)
{
    Tokens tokens = Tokens({
        Token(L_PAREN),
        Token(R_PAREN),
        Token(INTO_FILE),
        Token("file"),
        Token(EOF_TOKEN),
    });
    log_token_list(tokens.self);

    assert_unexpected_token_during_parsing(tokens);
}

TEST(Parser, RejectsWtfWithLeadingRedirectedSubshell)
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
    log_token_list(tokens.self);

    assert_unexpected_token_during_parsing(tokens);
}
