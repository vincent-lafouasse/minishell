#include "gtest/gtest.h"

#include <cstring>
#include <vector>

extern "C"
{
#include "log/log.h"
#include "parse/parse.h"
};

TEST(ParserIntegration, NoCommand)
{
    ASSERT_TRUE(1 == 1);
}
