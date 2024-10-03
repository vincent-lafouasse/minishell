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

// ------------------ SIMPLES ------------------

TEST(ParserIntegration, SimpleWord)
{
	const char *input = "echo";
    t_command expected = command_new_simple(Words({"echo"}).get(), nullptr);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWords)
{
	const char *input = "echo hello world";
	t_command expected = command_new_simple(Words({"echo", "hello", "world"}).get(), nullptr);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndTrailingInputRedirection) {
	const char *input = "echo hello world < infile";
	t_command expected = command_new_simple(
				Words({"echo", "hello", "world"}).get(),
				Redirections({FromFile("infile")}).get());

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndTrailingOutputRedirection) {
	const char* input = "echo hello world > outfile";
	t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).get(),
		Redirections({IntoFile("outfile")}).get()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndTrailingAppendRedirection) {
    const char *input = "echo hello world >> appendfile";
    t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).get(),
        Redirections({AppendIntoFile("appendfile")}).get()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndTrailingHereDocument) {
    const char *input = "echo hello world << eof";
    t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).get(),
        Redirections({HereDoc("eof")}).get()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndAllTrailingRedirections) {
	const char* input = "echo hello world > outfile < infile >> appendfile << eof";
	t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).get(),
		Redirections({IntoFile("outfile"), FromFile("infile"), AppendIntoFile("appendfile"), HereDoc("eof")}).get()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndLeadingInputRedirection) {
	const char* input = "< infile echo hello world";
	t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).get(),
		Redirections({FromFile("infile")}).get()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndLeadingOutputRedirection) {
	const char* input = "> outfile echo hello world";
	t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).get(),
		Redirections({IntoFile("outfile")}).get()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndLeadingAppendRedirection) {
	const char* input = ">> appendfile echo hello world";
}
// << eof echo hello world
TEST(ParserIntegration, SimpleWordsAndLeadingHereDocument) {}
// > outfile < infile >> appendfile << eof echo hello world
TEST(ParserIntegration, SimpleWordsAndAllLeadingRedirections) {}

// > outfile echo < infile hello >> appendfile world << eof
TEST(ParserIntegration, SimpleWordsMixedWithAllLeadingRedirections) {}

// < infile
TEST(ParserIntegration, SimpleRedirection) {}
// > outfile < infile >> appendfile << eof
TEST(ParserIntegration, SimpleAllRedirections) {}

// ------------------ PIPELINES ------------------

// cat /etc/passwd | sort
TEST(ParserIntegration, SimplePipeline) {}
// cat /etc/passwd | sort | head -n3
TEST(ParserIntegration, MultiPipeline) {}
// cat /etc/passwd | < Makefile sort | head -n3 > outfile
TEST(ParserIntegration, PipelineAndRedirections) {}
// < infile sort | head > outfile
TEST(ParserIntegration, PipelineWithTrailingLeadingRedirections) {}
// < infile sort | grep 'c' | cut -d: | head > outfile
TEST(ParserIntegration, MultiPipelineWithTrailingLeadingRedirections) {}

// ------------------ SUBSHELLS ------------------

// (echo hello world)
TEST(ParserIntegration, SimpleSubshell) {}
// (cat /etc/passwd | sort)
TEST(ParserIntegration, SubshellWithPipeline) {}
// (cat /etc/passwd | sort) > outfile
TEST(ParserIntegration, SubshellWithTrailingRedirection) {}
// (cat /etc/passwd | sort) > outfile < infile >> appendfile
TEST(ParserIntegration, SubshellInPipeline) {}
// ((cat /etc/passwd | grep a) | sort | grep abc)
TEST(ParserIntegration, SubshellWithTrailingRedirections) {}
// cat /etc/passwd | (sort | grep abc) | wc
TEST(ParserIntegration, SimpleNestedSubshellPipeline) {}
// (a | (b | (c | (d | (e | (f | g))))))
TEST(ParserIntegration, NestedSubshellPipelines) {}

// ------------------ CONDITIONALS ------------------

// false || true
TEST(ParserIntegration, SimpleCommandOrConditional) {}
// true && true
TEST(ParserIntegration, SimpleCommandAndConditional) {}
// false || false || true
TEST(ParserIntegration, ManySimplesOrConditional) {}
// true && true && false
TEST(ParserIntegration, ManySimplesAndConditional) {}
// true && true || false
TEST(ParserIntegration, ManySimplesAndOrConditional) {}
// true | true && false | true
TEST(ParserIntegration, PipelinesAndConditional) {}
// true | false || false | true
TEST(ParserIntegration, PipelinesOrConditional) {}
// true || (false | true)
TEST(ParserIntegration, SimplesAndSubshellsAndConditional) {}
// (abc) || thing | (echo hello && world)
TEST(ParserIntegration, SubshellsAndPipelinesConditional) {}
// (bat cat) << bash || (sh && (dash)) | (ulimit) && idk << im just > saying | stuff
TEST(ParserIntegration, AllCommandTypes) {}

// ------------------ PARSING ERRORS ------------------

// < infile (hello world)
TEST(ParserIntegration, RejectsSubshellWithLeadingRedirections) {}
// (abc | (def)
TEST(ParserIntegration, RejectsNoMatchingSubshellParen) {}
// cat abc | grep def |
TEST(ParserIntegration, RejectsUnterminatedPipeline) {}
// (abc def | ghi
TEST(ParserIntegration, RejectsUnterminatedSubshell) {}
// echo hello world < > outfile
TEST(ParserIntegration, RejectsRedirectionWithNoFollowingWord) {}
// (  )
TEST(ParserIntegration, RejectsEmptySubshell) {}
// (  ) > outfile
TEST(ParserIntegration, RejectsEmptySubshellWithTrailingRedirection) {}
// (bat cat) << bash || (sh && (dash)) | << woops (ulimit) && idk << im just > saying | stuff
TEST(ParserIntegration, RejectsWtfWithLeadingRedirectedSubshell) {}
