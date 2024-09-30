#include "gtest/gtest.h"

#include <vector>

extern "C"
{
#include "parse/parse.h"
};


void assert_successful_parsing(t_token_list* tokens)
{
    t_command parsed = parse_command(tokens);

    SUCCEED();
}

TEST(Parser, Dummy)
{
    SUCCEED();
}
