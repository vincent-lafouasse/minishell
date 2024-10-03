#include "parse/t_command/t_command.h"
#include "gtest/gtest.h"

#include <assert.h>
#include <cstring>
#include <vector>

#include "Words.h"

extern "C"
{
#include "parse/parse.h"

#include "word/t_word_list/t_word_list.h"
#include "log/log.h"
};

bool command_eq(t_command a, t_command b) {
	if (a.type != b.type)
		return false;
	if (a.type == SIMPLE_CMD) {
		if (Words(a.simple) != Words(b.simple))
			return false;
		if (!redirections_eq(a.simple->redirections, b.simple->redirections))
			return false;
		return true;
	} else if (a.type == SUBSHELL_CMD) {
		if (!redirections_eq(a.subshell->redirections, b.subshell->redirections))
			return false;
		return command_eq(a.subshell->cmd, b.subshell->cmd);
	} else if (a.type == CONDITIONAL_CMD) {
		if (a.conditional->op != b.conditional->op)
			return false;
		return command_eq(a.conditional->first, b.conditional->first) &&
			command_eq(a.conditional->second, b.conditional->second);
	} else if (a.type == PIPELINE_CMD) {
		return command_eq(a.pipeline->first, b.pipeline->first) &&
			command_eq(a.pipeline->second, b.pipeline->second);
	}
	assert(false);
}

TEST(ParserIntegration, SimpleWords)
{
	const char *input = "echo hello world";
	t_command actual;
	t_command expected;
	t_error err;

	err = parse(input, &actual);
	ASSERT_EQ(err, NO_ERROR);

	expected = command_new_simple(Words({"echo", "hello", "world"}), nullptr);
	ASSERT_TRUE(command_eq(actual, expected));
}
