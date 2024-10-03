#include "gtest/gtest.h"

#include <assert.h>
#include <cstring>
#include <vector>

#include "Words.h"
#include "Redirections.h"

extern "C"
{
#include "parse/parse.h"
#include "parse/t_command/t_command.h"

#include "word/t_word_list/t_word_list.h"
#include "log/log.h"
};

bool command_eq(t_command a, t_command b) {
	if (a.type != b.type)
		return false;
	if (a.type == SIMPLE_CMD) {
		if (Words(a.simple) != Words(b.simple))
			return false;
		if (Redirections(a.simple) != Redirections(b.simple))
			return false;
		return true;
	} else if (a.type == SUBSHELL_CMD) {
		if (Redirections(a.subshell) != Redirections(b.subshell))
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

TEST(ParserIntegration, SimpleWord)
{
	const char *input = "echo";
	t_command actual;
	t_command expected;
	t_error err;

	err = parse(input, &actual);
	ASSERT_EQ(err, NO_ERROR);

	expected = command_new_simple(Words({"echo"}).get(), nullptr);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWords)
{
	const char *input = "echo hello world";
	t_command actual;
	t_command expected;
	t_error err;

	err = parse(input, &actual);
	ASSERT_EQ(err, NO_ERROR);

	expected = command_new_simple(Words({"echo", "hello", "world"}).get(), nullptr);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndTrailingInputRedirection) {}
TEST(ParserIntegration, SimpleWordsAndTrailingOutputRedirection) {}
TEST(ParserIntegration, SimpleWordsAndTrailingAppendRedirection) {}
TEST(ParserIntegration, SimpleWordsAndTrailingHereDocument) {}
TEST(ParserIntegration, SimpleWordsAndAllTrailingRedirections) {}

TEST(ParserIntegration, SimpleWordsAndLeadingInputRedirection) {}
TEST(ParserIntegration, SimpleWordsAndLeadingOutputRedirection) {}
TEST(ParserIntegration, SimpleWordsAndLeadingAppendRedirection) {}
TEST(ParserIntegration, SimpleWordsAndLeadingHereDocument) {}
TEST(ParserIntegration, SimpleWordsAndAllLeadingRedirections) {}

TEST(ParserIntegration, SimpleWordsMixedWithAllLeadingRedirections) {}

TEST(ParserIntegration, SimpleRedirection) {}
TEST(ParserIntegration, SimpleAllRedirections) {}

TEST(ParserIntegration, SimplePipeline) {}
TEST(ParserIntegration, MultiPipeline) {}
TEST(ParserIntegration, PipelineAndRedirections) {}
TEST(ParserIntegration, PipelineWithTrailingLeadingRedirections) {}
TEST(ParserIntegration, MultiPipelineWithTrailingLeadingRedirections) {}

TEST(ParserIntegration, SimpleSubshell) {}
TEST(ParserIntegration, SubshellWithPipeline) {}
TEST(ParserIntegration, SubshellWithTrailingRedirection) {}
TEST(ParserIntegration, SubshellWithTrailingRedirections) {}
TEST(ParserIntegration, SubshellInPipeline) {}
TEST(ParserIntegration, SimpleNestedSubshell) {}
TEST(ParserIntegration, NestedSubshellPipelines) {}

TEST(ParserIntegration, SimpleCommandOrConditional) {}
TEST(ParserIntegration, SimpleCommandAndConditional) {}
TEST(ParserIntegration, ManySimplesOrConditional) {}
TEST(ParserIntegration, ManySimplesAndConditional) {}
TEST(ParserIntegration, PipelinesAndConditional) {}
TEST(ParserIntegration, PipelinesOrConditional) {}
TEST(ParserIntegration, SimplesAndSubshellsAndConditional) {}
TEST(ParserIntegration, SubshellsAndPipelinesConditional) {}

TEST(ParserIntegration, RejectsRedirectionWithoutWord) {}
TEST(ParserIntegration, RejectsSubshellWithLeadingRedirections) {}
TEST(ParserIntegration, RejectsNoMatchingSubshellParen) {}
TEST(ParserIntegration, RejectsUnterminatedPipeline) {}
TEST(ParserIntegration, RejectsUnterminatedSubshell) {}
TEST(ParserIntegration, RejectsRedirectionWithNoFollowingWord) {}
TEST(ParserIntegration, RejectsEmptySubshell) {}
TEST(ParserIntegration, RejectsEmptySubshellWithTrailingRedirection) {}
TEST(ParserIntegration, RejectsWtfWithLeadingRedirectedSubshell) {}
