#include "gtest/gtest.h"

#include "Variables.h"
#include "Words.h"

extern "C"
{
#include "word/expansions/expand.h"
};

TEST(WordListExpansion, SingleUnquotedWord) // WordExpansion or WordListExpansion?
{
	Variables variables = {
		{}
	};
	Words input = {{"hello"}};
	Words actual = input;
	Words expected = {{"hello"}};

	ASSERT_TRUE(
		variable_expand_words(variables.as_c_struct(), (t_word_list **)&actual.words) == NO_ERROR
	);
	ASSERT_TRUE(actual == expected) << "expected: " << expected << "got: "<< actual;
}

TEST(WordListExpansion, ExpandsEcho)
{
	Variables variables = {
		{
			{"LETTER_H", "h"},
		}
	};
	Words input = {{"e'c'$LETTER_H\"o\"", "hello", "world"}};
	Words actual = input;
	Words expected = {{"echo", "hello", "world"}};

	ASSERT_TRUE(
		variable_expand_words(variables.as_c_struct(), (t_word_list **)&actual.words) == NO_ERROR
	);
	ASSERT_TRUE(actual == expected) << "expected: " << expected << "got: "<< actual;
}

TEST(WordListExpansion, ExpandsTwoUnquotedVariableWords)
{
	Variables variables = {
		{
			{"GREETING", "hello"},
			{"NAME", "world"},
		}
	};
	Words input = {{"$GREETING", "$NAME"}};
	Words actual = input;
	Words expected = {{"hello", "world"}};

	ASSERT_TRUE(
		variable_expand_words(variables.as_c_struct(), (t_word_list **)&actual.words) == NO_ERROR
	);
	ASSERT_TRUE(actual == expected) << "expected: " << expected << "got: "<< actual;
}
