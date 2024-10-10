#include "gtest/gtest.h"

#include <string>
#include <vector>

extern "C"
{
#include "parse/tokenize/t_token.h"
#include "parse/tokenize/tokenize.h"
#include "parse/tokenize/t_token_list/t_token_list.h"
#include "error/t_error.h"
#include "log/log.h"
};

const char* log_tkl_wrapper(const t_token_list* tks)
{
    log_token_list(tks);
    return "";
}

static void assert_tkl_equality(const t_token_list *tokens,
                                const std::vector<t_token> &expected_tokens)
{
    const auto head = tokens;
    auto expected_it = expected_tokens.cbegin();

    while (expected_it != expected_tokens.cend() && tokens)
    {
        const t_token expected = *expected_it;
        const t_token actual = tokens->token;

        ASSERT_EQ(expected.type, actual.type)
            << "Mismatched token types, expected " << token_repr(expected)
            << " was " << token_repr(actual) << log_tkl_wrapper(head);
        ASSERT_STREQ(expected.literal, actual.literal) << log_tkl_wrapper(head);
        expected_it++;
        tokens = tokens->next;
    }
    ASSERT_EQ(tokens, nullptr) << log_tkl_wrapper(head);
    ASSERT_EQ(expected_it, expected_tokens.cend()) << log_tkl_wrapper(head);
}

static void assert_lexing_ok(const char* input, const std::vector<t_token>& expected)
{
    t_token_list* actual;
    
    t_error err = tokenize(input, &actual);
    ASSERT_EQ(err, NO_ERROR) << "Lexing failed with error code " << error_repr(err);
    assert_tkl_equality(actual, expected);
    tkl_clear(&actual, free);
}

static void assert_lexing_fail(const char* input, t_error expected)
{
    t_token_list* tokens;
    
    t_error actual = tokenize(input, &tokens);
    ASSERT_EQ(actual, expected) << "Expected error code " << error_repr(expected) << " was " << error_repr(actual);
}

static t_token Token(t_token_type type)
{
    assert(type != WORD);
    return (t_token){.type = type, .literal = nullptr};
}

static t_token Token(const char *literal)
{
    assert(literal != nullptr);
    return (t_token){.type = WORD, .literal = const_cast<char *>(literal)};
}

TEST(Tokenize, Word) {
    const char *input = "hello";
    std::vector<t_token> expected = {
        Token("hello"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, SeparatedWords) {
    const char *input = "echo     \n hello  \tworld";
    std::vector<t_token> expected = {
        Token("echo"),
        Token("hello"),
        Token("world"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, QuotedWord) {
    const char *input = "'hello'";
    std::vector<t_token> expected = {
        Token("'hello'"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, DoubleQuotedWord) {
    const char *input = "\"hello\"";
    std::vector<t_token> expected = {
        Token("\"hello\""),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, SeparatedQuotedWords) {
    const char *input = "'echo' \n \"hello\" 'world'";
    std::vector<t_token> expected = {
        Token("'echo'"),
        Token("\"hello\""),
        Token("'world'"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, SeparatedQuotedAndUnquotedWords) {
    const char *input = "echo 'hello' world";
    std::vector<t_token> expected = {
        Token("echo"),
        Token("'hello'"),
        Token("world"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, JoinedQuotedAndUnquotedWords) {
    const char *input = "'he'l\"lo\"world";
    std::vector<t_token> expected = {
        Token("'he'l\"lo\"world"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, JoinedQuotedWords) {
    const char *input = "'hello'\"world\"";
    std::vector<t_token> expected = {
        Token("'hello'\"world\""),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, SeparatedJoinedQuotedAndUnquotedWords) {
    const char *input = "e\"c\"h'o' 'he'l\"lo\" world";
    std::vector<t_token> expected = {
        Token("e\"c\"h'o'"),
        Token("'he'l\"lo\""),
        Token("world"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, RejectsUnterminatedQuotes) {
    const char *input = "echo 'hello world";
    t_error expected = E_UNTERMINATED_QUOTE;

    assert_lexing_fail(input, expected);
}

TEST(Tokenize, RejectsJoinedWordsAndUnterminatedQuotes) {
    const char *input = "echo hel'lo world";
    t_error expected = E_UNTERMINATED_QUOTE;

    assert_lexing_fail(input, expected);
}

TEST(Tokenize, QuotesEscapeOperators) {
    const char *input = "<< hello world || abc";
    std::vector<t_token> expected = {
        Token(DL_ANGLE_BRACKET),
        Token("hello"),
        Token("world"),
        Token(OR_OR),
        Token("abc"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, WordsAndRedirectionOperators) {
    const char *input = "<< eof < infile hello > outfile";
    std::vector<t_token> expected = {
        Token(DL_ANGLE_BRACKET),
        Token("eof"),
        Token(L_ANGLE_BRACKET),
        Token("infile"),
        Token("hello"),
        Token(R_ANGLE_BRACKET),
        Token("outfile"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, WordsAndLogicalOperators) {
    const char *input = "hello && world || true";
    std::vector<t_token> expected = {
        Token("hello"),
        Token(AND_AND),
        Token("world"),
        Token(OR_OR),
        Token("true"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, WordsAndControlOperators) {
    const char *input = "hello  | | world |";
    std::vector<t_token> expected = {
        Token("hello"),
        Token(PIPE),
        Token(PIPE),
        Token("world"),
        Token(PIPE),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, WordsAndParens) {
    const char *input = "(ulimit abc) hello words";
    std::vector<t_token> expected = {
        Token(L_PAREN),
        Token("ulimit"),
        Token("abc"),
        Token(R_PAREN),
        Token("hello"),
        Token("words"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, JoinedWordsAndRedirectionOperators) {
    const char *input = "<<'e'of joined\"he\"ll'o '>outfile";
    std::vector<t_token> expected = {
        Token(DL_ANGLE_BRACKET),
        Token("'e'of"),
        Token("joined\"he\"ll'o '"),
        Token(R_ANGLE_BRACKET),
        Token("outfile"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, JoinedWordsAndLogicalOperators) {
    const char *input = "he\"llo\"&&'world'\"\"||true";
    std::vector<t_token> expected = {
        Token("he\"llo\""),
        Token(AND_AND),
        Token("'world'\"\""),
        Token(OR_OR),
        Token("true"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, JoinedWordsAndControlOperators) {
    const char *input = "hel'lo'|'world'";
    std::vector<t_token> expected = {
        Token("hel'lo'"),
        Token(PIPE),
        Token("'world'"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, JoinedWordsAndParens) {
    const char *input = "(hello'world')";
    std::vector<t_token> expected = {
        Token(L_PAREN),
        Token("hello'world'"),
        Token(R_PAREN),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, SeparatedOperators) {
    const char *input = "( << <\t >> >\n|| &&\t)";
    std::vector<t_token> expected = {
        Token(L_PAREN),
        Token(DL_ANGLE_BRACKET),
        Token(L_ANGLE_BRACKET),
        Token(DR_ANGLE_BRACKET),
        Token(R_ANGLE_BRACKET),
        Token(OR_OR),
        Token(AND_AND),
        Token(R_PAREN),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, JoinedOperators) {
    const char *input = "(<<<>>>||&&)";
    std::vector<t_token> expected = {
        Token(L_PAREN),
        Token(DL_ANGLE_BRACKET),
        Token(L_ANGLE_BRACKET),
        Token(DR_ANGLE_BRACKET),
        Token(R_ANGLE_BRACKET),
        Token(OR_OR),
        Token(AND_AND),
        Token(R_PAREN),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, WordsAndOperatorsInNestedParens) {
    const char *input = "(a | (b | (c)))";
    std::vector<t_token> expected = {
        Token(L_PAREN),
        Token("a"),
        Token(PIPE),
        Token(L_PAREN),
        Token("b"),
        Token(PIPE),
        Token(L_PAREN),
        Token("c"),
        Token(R_PAREN),
        Token(R_PAREN),
        Token(R_PAREN),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, WordsAndControlPlusLogicalOperators) {
    const char *input = "echo abc && cat Makefile | grep c | wc -l || false";
    std::vector<t_token> expected = {
        Token("echo"),
        Token("abc"),
        Token(AND_AND),
        Token("cat"),
        Token("Makefile"),
        Token(PIPE),
        Token("grep"),
        Token("c"),
        Token(PIPE),
        Token("wc"),
        Token("-l"),
        Token(OR_OR),
        Token("false"),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, WordsAndAllOperators) {
    const char *input = "'word1' << word2 <>>>|| word3&&";
    std::vector<t_token> expected = {
        Token("'word1'"),
        Token(DL_ANGLE_BRACKET),
        Token("word2"),
        Token(L_ANGLE_BRACKET),
        Token(DR_ANGLE_BRACKET),
        Token(R_ANGLE_BRACKET),
        Token(OR_OR),
        Token("word3"),
        Token(AND_AND),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, WordsAndAllOperatorsPlusParens) {
    const char *input = "('word1' << word2 <>>>|| word3&&)";
    std::vector<t_token> expected = {
        Token(L_PAREN),
        Token("'word1'"),
        Token(DL_ANGLE_BRACKET),
        Token("word2"),
        Token(L_ANGLE_BRACKET),
        Token(DR_ANGLE_BRACKET),
        Token(R_ANGLE_BRACKET),
        Token(OR_OR),
        Token("word3"),
        Token(AND_AND),
        Token(R_PAREN),
        Token(EOF_TOKEN)
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, SeparatorsYieldEmptyTokenList) {
    const char *input = "     \t\n             \t \t";
    std::vector<t_token> expected = {Token(EOF_TOKEN)};

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, Pipex)
{
    const char *input = "< infile cmd1 | cmd2 > outfile";
    std::vector<t_token> expected = {
        Token(L_ANGLE_BRACKET),
        Token("infile"),
        Token("cmd1"),
        Token(PIPE),
        Token("cmd2"),
        Token(R_ANGLE_BRACKET),
        Token("outfile"),
        Token(EOF_TOKEN),
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, BigPipex)
{
    const char *input =
        "cppcheck < file.cpp | cat | sort | wc | a_cmd >> outfile";
    std::vector<t_token> expected = {
        Token("cppcheck"), Token(L_ANGLE_BRACKET),
        Token("file.cpp"), Token(PIPE),
        Token("cat"),      Token(PIPE),
        Token("sort"),     Token(PIPE),
        Token("wc"),       Token(PIPE),
        Token("a_cmd"),    Token(DR_ANGLE_BRACKET),
        Token("outfile"),  Token(EOF_TOKEN),
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, BigPipexNoWhitespace)
{
    const char *input = "cppcheck<file.cpp|cat|sort|wc|a_cmd>>outfile";
    std::vector<t_token> expected = {
        Token("cppcheck"), Token(L_ANGLE_BRACKET),
        Token("file.cpp"), Token(PIPE),
        Token("cat"),      Token(PIPE),
        Token("sort"),     Token(PIPE),
        Token("wc"),       Token(PIPE),
        Token("a_cmd"),    Token(DR_ANGLE_BRACKET),
        Token("outfile"),  Token(EOF_TOKEN),
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, BigPipexLotsOfWhitespace)
{
    const char *input = "   cppcheck   <   file.cpp   |   cat | sort | wc | "
                         "a_cmd >> outfile   ";
    std::vector<t_token> expected = {
        Token("cppcheck"), Token(L_ANGLE_BRACKET),
        Token("file.cpp"), Token(PIPE),
        Token("cat"),      Token(PIPE),
        Token("sort"),     Token(PIPE),
        Token("wc"),       Token(PIPE),
        Token("a_cmd"),    Token(DR_ANGLE_BRACKET),
        Token("outfile"),  Token(EOF_TOKEN),
    };

    assert_lexing_ok(input, expected);
}

TEST(Tokenize, RejectsUnrecognizedOperator) {
    const char *input = "abc & defg";
    t_error expected = E_UNRECOGNIZED_TOKEN;

    assert_lexing_fail(input, expected);
}
