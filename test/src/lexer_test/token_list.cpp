#include "gtest/gtest.h"

#include <vector>

extern "C"
{
#include "parse/tokenize/t_token_list/t_token_list.h"
};

TEST(TokenList, PushBack)
{
    std::vector<t_token> expected = {
        (t_token){.type = WORD, .literal = (char *)"hello"},
        (t_token){.type = WORD, .literal = (char *)" "},
        (t_token){.type = WORD, .literal = (char *)"world"},
    };

    t_token_list *actual = NULL;

    for (const t_token &token : expected)
    {
        tkl_push_back(&actual, token);
    }

    t_token_list *current = actual;
    auto it = expected.cbegin();

    while (it != expected.cend() && current)
    {
        ASSERT_EQ(it->type, current->token.type);
        ASSERT_STREQ(it->literal, current->token.literal);
        it++;
        current = current->next;
    }
    ASSERT_EQ(it, expected.cend());
    ASSERT_EQ(current, nullptr);
    tkl_clear(&actual, NULL);
}
