#include "parse/t_symbol.h"
#include "gtest/gtest.h"

#include <vector>

extern "C"
{
#include "parse/parse.h"
};


void assert_successful_parsing(t_token_list* tokens)
{
    t_symbol sym;
    t_command parsed;
    t_error err = parse_command(tokens, &sym);

    ASSERT_EQ(err, NO_ERROR);
}

TEST(Parser, Dummy)
{
    SUCCEED();
}
