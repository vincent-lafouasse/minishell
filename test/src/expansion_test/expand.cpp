#include "gtest/gtest.h"

#include "Variables.h"

#include <cstring>

extern "C"
{
#include "word/expansions/expand.h"
};

TEST(WordListExpansion, SingleWord)
{
	Variables variables = {{}};

	variable_expand_words(variables.as_c_struct(), nullptr);
}
