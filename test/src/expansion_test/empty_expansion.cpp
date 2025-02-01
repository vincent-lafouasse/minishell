#include "gtest/gtest.h"

#include "Variables.h"
#include "Words.h"

extern "C"
{
#include "word/expansions/expand.h"
};

TEST(EmptyWordListExpansion, ManyEmptyVariablesAreElidedFromStartOfWordList)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	Words input = {{"$EMPTY", "$EMPTY", "hello", "$EMPTY"}};
	Words actual = input;
	Words expected = {{"hello"}};

	ASSERT_TRUE(
		variable_expand_words(variables.as_c_struct(), (t_word_list **)&actual.words) == NO_ERROR
	);
	ASSERT_TRUE(actual == expected) << "expected: " << expected << "got: "<< actual;
}

TEST(EmptyWordListExpansion, EmptyVariablesAreElidedFromMiddleOfWordList)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	Words input = {{"echo", "$EMPTY", "hello", "$EMPTY", "world"}};
	Words actual = input;
	Words expected = {{"echo", "hello", "world"}};

	ASSERT_TRUE(
		variable_expand_words(variables.as_c_struct(), (t_word_list **)&actual.words) == NO_ERROR
	);
	ASSERT_TRUE(actual == expected) << "expected: " << expected << "got: "<< actual;
}

TEST(EmptyWordExpansion, OneEmptyVariableExpandsToNoWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = "$EMPTY";
	char *actual = nullptr;

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_EQ(actual, nullptr) << "expected no word and got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, ManyEmptyVariablesExpandToNoWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = "$EMPTY$EMPTY$EMPTY";
	char *actual = nullptr;

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_EQ(actual, nullptr) << "expected no word and got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, OneNonExistentVariableExpandsToNoWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = "$DNE";
	char *actual = nullptr;

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_EQ(actual, nullptr) << "expected no word and got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, EmptyQuotedStringExpandsToEmptyWord)
{
	Variables variables = {
		{}
	};
	const char *input = "\"\"";
	char *actual = nullptr;
	const char *expected = "";

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_STREQ(actual, expected) << "expected: " << expected << "got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, OneQuotedNonExistentVariableExpandsToEmptyWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = "\"$DNE\"";
	char *actual = nullptr;
	const char *expected = "";

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_STREQ(actual, expected) << "expected: " << expected << "got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, TwoQuotedEmptyVariablesExpandToOneEmptyWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = "\"$EMPTY\"\"$EMPTY\"";
	char *actual = nullptr;
	const char *expected = "";

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_STREQ(actual, expected) << "expected: " << expected << "got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, TwoEmptyQuotedStringsExpandToOneEmptyWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = "\'\'\"\"";
	char *actual = nullptr;
	const char *expected = "";

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_STREQ(actual, expected) << "expected: " << expected << "got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, EmptyStringFollowedByNonExistentVariableExpandsToEmptyWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = "\"\"$DNE";
	char *actual = nullptr;
	const char *expected = "";

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_STREQ(actual, expected) << "expected: " << expected << "got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, EmptyVariableSurroundedByEmptyStringsExpandsToEmptyWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = "\"\"$EMPTY\"\"";
	char *actual = nullptr;
	const char *expected = "";

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_STREQ(actual, expected) << "expected: " << expected << "got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, EmptyStringExpandsToEmptyWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = R"("")";
	char *actual = nullptr;
	const char *expected = "";

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_STREQ(actual, expected) << "expected: " << expected << "got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, EmptyVariableFollowedByEmptyStringExpandsToEmptyWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = R"($EMPTY"")";
	char *actual = nullptr;
	const char *expected = "";

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_STREQ(actual, expected) << "expected: " << expected << "got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, EmptyStringFollowedByEmptyVariableExpandsToEmptyWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = R"(""$EMPTY)";
	char *actual = nullptr;
	const char *expected = "";

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_STREQ(actual, expected) << "expected: " << expected << "got: " << actual;
	free(actual);
}

TEST(EmptyWordExpansion, EmptyStringSurroundedByEmptyVariablesExpandsToEmptyWord)
{
	Variables variables = {
		{{"EMPTY", ""}}
	};
	const char *input = R"($EMPTY""$EMPTY)";
	char *actual = nullptr;
	const char *expected = "";

	ASSERT_EQ(variable_expand_word(variables.as_c_struct(), input, &actual), NO_ERROR);
	ASSERT_STREQ(actual, expected) << "expected: " << expected << "got: " << actual;
	free(actual);
}

