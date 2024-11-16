#include "gtest/gtest.h"

#include <vector>
#include <string>

extern "C"
{
#include "word/expansions/t_word_quotes_list.h"
};

static bool word_quotes_list_equals(t_word_quotes_list *actual, \
                                    std::vector<t_word_quotes_list> expected)
{
    for (auto expected_wql : expected)
    {
        if (actual == nullptr)
            return (false);
        if (strcmp(actual->part, expected_wql.part) != 0)
            return (false);
        if (actual->state != expected_wql.state)
            return (false);
        actual = actual->next;
    }
    bool at_end = actual == nullptr;
    return (at_end);
}

t_word_quotes_list word_quotes_list(const char *part, t_word_quote_state state)
{
    return ((t_word_quotes_list){.part = const_cast<char *>(part), .state = state});
}

TEST(WordQuotesListParsing, ParsesSingleUnquotedString)
{
    std::vector<t_word_quotes_list> expected = {
        word_quotes_list("hello", WQS_UNQUOTED)
    };

    const char *input = "hello";

    t_word_quotes_list *actual = nullptr;
    ASSERT_EQ(wql_parse(input, &actual), NO_ERROR);
    ASSERT_NE(actual, nullptr);
    ASSERT_TRUE(word_quotes_list_equals(actual, expected));
    wql_clear(&actual);
}

TEST(WordQuotesListParsing, ParsesSingleDoublyQuotedString)
{
    std::vector<t_word_quotes_list> expected = {
        word_quotes_list("\"hello world\"", WQS_DOUBLY_QUOTED)
    };

    const char *input = "\"hello world\"";

    t_word_quotes_list *actual = nullptr;
    ASSERT_EQ(wql_parse(input, &actual), NO_ERROR);
    ASSERT_NE(actual, nullptr);
    ASSERT_TRUE(word_quotes_list_equals(actual, expected));
    wql_clear(&actual);
}

TEST(WordQuotesListParsing, ParsesSingleSinglyQuotedString)
{
    std::vector<t_word_quotes_list> expected = {
        word_quotes_list("'hello world'", WQS_SINGLY_QUOTED)
    };

    const char *input = "'hello world'";

    t_word_quotes_list *actual = nullptr;
    ASSERT_EQ(wql_parse(input, &actual), NO_ERROR);
    ASSERT_NE(actual, nullptr);
    ASSERT_TRUE(word_quotes_list_equals(actual, expected));
    wql_clear(&actual);
}

TEST(WordQuotesListParsing, ParsesManyJoinedSameQuotedStrings)
{
    std::vector<t_word_quotes_list> expected = {
        word_quotes_list("'abc'", WQS_SINGLY_QUOTED),
        word_quotes_list("'def'", WQS_SINGLY_QUOTED),
    };

    const char *input = "'abc''def'";

    t_word_quotes_list *actual = nullptr;
    ASSERT_EQ(wql_parse(input, &actual), NO_ERROR);
    ASSERT_NE(actual, nullptr);
    ASSERT_TRUE(word_quotes_list_equals(actual, expected));
    wql_clear(&actual);
}

TEST(WordQuotesListParsing, ParsesManyJoinedQuotedStrings)
{
    std::vector<t_word_quotes_list> expected = {
        word_quotes_list("'hello'", WQS_SINGLY_QUOTED),
        word_quotes_list("\"world\"", WQS_DOUBLY_QUOTED),
        word_quotes_list("'abcdef'", WQS_SINGLY_QUOTED)
    };

    const char *input = "'hello'\"world\"'abcdef'";

    t_word_quotes_list *actual = nullptr;
    ASSERT_EQ(wql_parse(input, &actual), NO_ERROR);
    ASSERT_NE(actual, nullptr);
    ASSERT_TRUE(word_quotes_list_equals(actual, expected));
    wql_clear(&actual);
}

TEST(WordQuotesListParsing, ParsesManyJoinedQuotedAndUnquotedStrings)
{
    std::vector<t_word_quotes_list> expected = {
        word_quotes_list("$LETTER_E", WQS_UNQUOTED),
        word_quotes_list("\"c\"", WQS_DOUBLY_QUOTED),
        word_quotes_list("h", WQS_UNQUOTED),
        word_quotes_list("'o'", WQS_SINGLY_QUOTED)
    };

    const char *input = "$LETTER_E\"c\"h'o'";

    t_word_quotes_list *actual = nullptr;
    ASSERT_EQ(wql_parse(input, &actual), NO_ERROR);
    ASSERT_NE(actual, nullptr);
    ASSERT_TRUE(word_quotes_list_equals(actual, expected));
    wql_clear(&actual);
}

TEST(WordQuotesListParsing, ParsesManyJoinedQuotedThenUnquotedStrings)
{
    std::vector<t_word_quotes_list> expected = {
        word_quotes_list("hello", WQS_UNQUOTED),
        word_quotes_list("'world'", WQS_SINGLY_QUOTED)
    };

    const char *input = "hello'world'";

    t_word_quotes_list *actual = nullptr;
    ASSERT_EQ(wql_parse(input, &actual), NO_ERROR);
    ASSERT_NE(actual, nullptr);
    ASSERT_TRUE(word_quotes_list_equals(actual, expected));
    wql_clear(&actual);
}

TEST(WordQuotesListParsing, ParseEmptyStringReturnsNull)
{
    const char *input = "";

    t_word_quotes_list *actual = nullptr;
    ASSERT_EQ(wql_parse(input, &actual), NO_ERROR);
    ASSERT_EQ(actual, nullptr);
}

TEST(WordQuotesListParsing, ParsesEmptySinglyQuotedString)
{
    std::vector<t_word_quotes_list> expected = {
        word_quotes_list("''", WQS_SINGLY_QUOTED)
    };

    const char *input = "''";

    t_word_quotes_list *actual = nullptr;
    ASSERT_EQ(wql_parse(input, &actual), NO_ERROR);
    ASSERT_NE(actual, nullptr);
    ASSERT_TRUE(word_quotes_list_equals(actual, expected));
    wql_clear(&actual);
}

TEST(WordQuotesListParsing, ParsesEmptyDoublyQuotedString)
{
    std::vector<t_word_quotes_list> expected = {
        word_quotes_list("\"\"", WQS_DOUBLY_QUOTED)
    };

    const char *input = "\"\"";

    t_word_quotes_list *actual = nullptr;
    ASSERT_EQ(wql_parse(input, &actual), NO_ERROR);
    ASSERT_NE(actual, nullptr);
    ASSERT_TRUE(word_quotes_list_equals(actual, expected));
    wql_clear(&actual);
}
