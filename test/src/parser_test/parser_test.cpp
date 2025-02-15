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

const char* command_type_repr(t_command_type ty) {
	switch (ty) {
		case CMD_SIMPLE:
			return "Simple";
		case CMD_PIPELINE:
			return "Pipeline";
		case CMD_CONDITIONAL:
			return "Conditional";
		case CMD_SUBSHELL:
			return "Subshell";
	}
}

void assert_command_eq(t_command actual, t_command expected) {
	ASSERT_EQ(actual.type, expected.type) << "Expected " << command_type_repr(expected.type) << " was " << command_type_repr(actual.type);
	if (actual.type == CMD_SIMPLE) {
		ASSERT_EQ(Words(actual.simple), Words(expected.simple));
		ASSERT_EQ(Redirections(actual.simple), Redirections(expected.simple));
	} else if (actual.type == CMD_SUBSHELL) {
		ASSERT_EQ(Redirections(actual.subshell), Redirections(expected.subshell));
		assert_command_eq(actual.subshell->cmd, expected.subshell->cmd);
	} else if (actual.type == CMD_CONDITIONAL) {
		ASSERT_EQ(actual.conditional->op, expected.conditional->op);
		assert_command_eq(actual.conditional->first, expected.conditional->first);
		assert_command_eq(actual.conditional->second, expected.conditional->second);
	} else if (actual.type == CMD_PIPELINE) {
		assert_command_eq(actual.pipeline->first, expected.pipeline->first);
		assert_command_eq(actual.pipeline->second, expected.pipeline->second);
	} else FAIL() << "unreachable statement";
}

// ------------------ SIMPLES ------------------

TEST(ParserIntegration, SimpleWord)
{
	const char *input = "echo";
    t_command expected = command_new_simple(Words({"echo"}).to_list(), nullptr);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleWords)
{
	const char *input = "echo hello world";
	t_command expected = command_new_simple(Words({"echo", "hello", "world"}).to_list(), nullptr);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsAndTrailingInputRedirection) {
	const char *input = "echo hello world < infile";
	t_command expected = command_new_simple(
				Words({"echo", "hello", "world"}).to_list(),
				Redirections({FromFile("infile")}).to_list());

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsAndTrailingOutputRedirection) {
	const char* input = "echo hello world > outfile";
	t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).to_list(),
		Redirections({IntoFile("outfile")}).to_list()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsAndTrailingAppendRedirection) {
    const char *input = "echo hello world >> appendfile";
    t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).to_list(),
        Redirections({AppendIntoFile("appendfile")}).to_list()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsAndTrailingHereDocument) {
    const char *input = "echo hello world << eof";
    t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).to_list(),
        Redirections({HereDoc("eof")}).to_list()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsAndAllTrailingRedirections) {
	const char* input = "echo hello world > outfile < infile >> appendfile << eof";
	t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).to_list(),
		Redirections({IntoFile("outfile"), FromFile("infile"), AppendIntoFile("appendfile"), HereDoc("eof")}).to_list()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsAndLeadingInputRedirection) {
	const char* input = "< infile echo hello world";
	t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).to_list(),
		Redirections({FromFile("infile")}).to_list()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsAndLeadingOutputRedirection) {
	const char* input = "> outfile echo hello world";
	t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).to_list(),
		Redirections({IntoFile("outfile")}).to_list()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsAndLeadingAppendRedirection) {
	const char* input = ">> appendfile echo hello world";
	t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).to_list(),
		Redirections({AppendIntoFile("appendfile")}).to_list()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsAndLeadingHereDocument) {
		const char* input = "<< eof echo hello world";
		t_command expected = command_new_simple(
				Words({"echo", "hello", "world"}).to_list(),
				Redirections({HereDoc("eof")}).to_list()
		);

		t_command actual;
		ASSERT_EQ(parse(input, &actual), NO_ERROR);
		assert_command_eq(actual, expected);
		command_destroy(actual);
		command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsAndAllLeadingRedirections) {
		const char* input = "> outfile < infile >> appendfile << eof echo hello world";
		t_command expected = command_new_simple(
				Words({"echo", "hello", "world"}).to_list(),
				Redirections({
						IntoFile("outfile"),
						FromFile("infile"),
						AppendIntoFile("appendfile"),
						HereDoc("eof"),
				}).to_list()
		);

		t_command actual;
		ASSERT_EQ(parse(input, &actual), NO_ERROR);
		assert_command_eq(actual, expected);
		command_destroy(actual);
		command_destroy(expected);
}

TEST(ParserIntegration, SimpleWordsMixedWithAllLeadingRedirections) {
		const char* input = "> outfile echo < infile hello >> appendfile world << eof";
		t_command expected = command_new_simple(
				Words({"echo", "hello", "world"}).to_list(),
				Redirections({
						IntoFile("outfile"),
						FromFile("infile"),
						AppendIntoFile("appendfile"),
						HereDoc("eof"),
				}).to_list()
		);

		t_command actual;
		ASSERT_EQ(parse(input, &actual), NO_ERROR);
		assert_command_eq(actual, expected);
		command_destroy(actual);
		command_destroy(expected);
}

TEST(ParserIntegration, SimpleRedirection) {
		const char* input = "< infile";
		t_command expected = command_new_simple(
				nullptr,
				Redirections({
						FromFile("infile"),
				}).to_list()
		);

		t_command actual;
		ASSERT_EQ(parse(input, &actual), NO_ERROR);
		assert_command_eq(actual, expected);
		command_destroy(actual);
		command_destroy(expected);
}
TEST(ParserIntegration, SimpleAllRedirections) {
		const char* input = "> outfile < infile >> appendfile << eof";
		t_command expected = command_new_simple(
				nullptr,
				Redirections({
						IntoFile("outfile"),
						FromFile("infile"),
						AppendIntoFile("appendfile"),
						HereDoc("eof"),
				}).to_list()
		);

		t_command actual;
		ASSERT_EQ(parse(input, &actual), NO_ERROR);
		assert_command_eq(actual, expected);
		command_destroy(actual);
		command_destroy(expected);
}

// ------------------ PIPELINES ------------------

TEST(ParserIntegration, SimplePipeline) {
	const char *input = "cat /etc/passwd | sort";
	t_command expected = command_new_pipeline(
		command_new_simple(Words({"cat", "/etc/passwd"}).to_list(), nullptr),
		command_new_simple(Words({"sort"}).to_list(), nullptr)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, MultiPipeline) {
	const char *input = "cat /etc/passwd | sort | head -n3";
	t_command expected = command_new_pipeline(
		command_new_simple(Words({"cat", "/etc/passwd"}).to_list(), nullptr),
		command_new_pipeline(
			command_new_simple(Words({"sort"}).to_list(), nullptr),
			command_new_simple(Words({"head", "-n3"}).to_list(), nullptr)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, PipelineAndRedirections) {
	const char *input = "cat /etc/passwd | < Makefile sort | head -n3 > outfile";
	t_command expected = command_new_pipeline(
		command_new_simple(Words({"cat", "/etc/passwd"}).to_list(), nullptr),
		command_new_pipeline(
			command_new_simple(Words({"sort"}).to_list(), Redirections({FromFile("Makefile")}).to_list()),
			command_new_simple(Words({"head", "-n3"}).to_list(), Redirections({IntoFile("outfile")}).to_list())
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, PipelineWithTrailingLeadingRedirections) {
	const char *input = "< infile sort | head -n3 > outfile";
	t_command expected = command_new_pipeline(
		command_new_simple(Words({"sort"}).to_list(), Redirections({FromFile("infile")}).to_list()),
		command_new_simple(Words({"head", "-n3"}).to_list(), Redirections({IntoFile("outfile")}).to_list())
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, MultiPipelineWithTrailingLeadingRedirections) {
	const char *input = "< infile sort | grep c | cut -d: | head > outfile";
	t_command expected = command_new_pipeline(
		command_new_simple(Words({"sort"}).to_list(), Redirections({FromFile("infile")}).to_list()),
		command_new_pipeline(
			command_new_simple(Words({"grep", "c"}).to_list(), nullptr),
			command_new_pipeline(
				command_new_simple(Words({"cut", "-d:"}).to_list(), nullptr),
				command_new_simple(Words({"head"}).to_list(), Redirections({IntoFile("outfile")}).to_list())
			)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

// ------------------ SUBSHELLS ------------------

TEST(ParserIntegration, SimpleSubshell) {
	const char *input = "(echo hello world)";
	t_command expected = command_new_subshell(
		command_new_simple(Words({"echo", "hello", "world"}).to_list(), nullptr),
		nullptr
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SubshellWithPipeline) {
	const char *input = "(cat /etc/passwd | sort)";
	t_command expected = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"cat", "/etc/passwd"}).to_list(), nullptr),
			command_new_simple(Words({"sort"}).to_list(), nullptr)
		),
		nullptr
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SubshellWithTrailingRedirection) {
	const char *input = "(cat /etc/passwd | sort) > outfile";
	t_command expected = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"cat", "/etc/passwd"}).to_list(), nullptr),
			command_new_simple(Words({"sort"}).to_list(), nullptr)
		),
		Redirections({IntoFile("outfile")}).to_list()
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SubshellWithAllTrailingRedirections) {
	const char *input = "(cat /etc/passwd | sort) > outfile < infile >> appendfile";
	t_command expected = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"cat", "/etc/passwd"}).to_list(), nullptr),
			command_new_simple(Words({"sort"}).to_list(), nullptr)
		),
		Redirections({IntoFile("outfile"), FromFile("infile"), AppendIntoFile("appendfile")}).to_list()
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SubshellInPipeline) {
	const char *input = "cat /etc/passwd | (sort | grep abc) | wc";
	t_command subshell = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"sort"}).to_list(), nullptr),
			command_new_simple(Words({"grep", "abc"}).to_list(), nullptr)
		),
		nullptr
	);
	t_command expected = command_new_pipeline(
		command_new_simple(Words({"cat", "/etc/passwd"}).to_list(), nullptr),
		command_new_pipeline(
			subshell,
			command_new_simple(Words({"wc"}).to_list(), nullptr)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleNestedSubshellPipeline) {
	const char *input = "((cat /etc/passwd | grep a) | sort | grep abc)";
	t_command subshell = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"cat", "/etc/passwd"}).to_list(), nullptr),
			command_new_simple(Words({"grep", "a"}).to_list(), nullptr)
		),
		nullptr
	);
	t_command pipeline = command_new_pipeline(
		subshell,
		command_new_pipeline(
			command_new_simple(Words({"sort"}).to_list(), nullptr),
			command_new_simple(Words({"grep", "abc"}).to_list(), nullptr)
		)
	);
	t_command expected = command_new_subshell(pipeline, nullptr);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, NestedSubshellPipelines) {
	const char *input = "(a | (b | (c | (d | (e | (f | g))))))";
	t_command fg = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"f"}).to_list(), nullptr),
			command_new_simple(Words({"g"}).to_list(), nullptr)
		),
		nullptr
	);
	t_command efg = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"e"}).to_list(), nullptr),
			fg
		),
		nullptr
	);
	t_command defg = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"d"}).to_list(), nullptr),
			efg
		),
		nullptr
	);
	t_command cdefg = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"c"}).to_list(), nullptr),
			defg
		),
		nullptr
	);
	t_command bcdefg = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"b"}).to_list(), nullptr),
			cdefg
		),
		nullptr
	);
	t_command expected = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"a"}).to_list(), nullptr),
			bcdefg
		),
		nullptr
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

// ------------------ CONDITIONALS ------------------

TEST(ParserIntegration, SimpleCommandOrConditional) {
	const char *input = "false || true";
	t_command expected = command_new_conditional(
		OR_OP,
		command_new_simple(Words({"false"}).to_list(), nullptr),
		command_new_simple(Words({"true"}).to_list(), nullptr)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimpleCommandAndConditional) {
	const char *input = "true && true";
	t_command expected = command_new_conditional(
		AND_OP,
		command_new_simple(Words({"true"}).to_list(), nullptr),
		command_new_simple(Words({"true"}).to_list(), nullptr)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, ManySimplesOrConditional) {
	const char *input = "false || false || true";
	t_command expected = command_new_conditional(
		OR_OP,
		command_new_conditional(
			OR_OP,
			command_new_simple(Words({"false"}).to_list(), nullptr),
			command_new_simple(Words({"false"}).to_list(), nullptr)
		),
		command_new_simple(Words({"true"}).to_list(), nullptr)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, ManySimplesAndConditional) {
	const char *input = "true && true && false";
	t_command expected = command_new_conditional(
		AND_OP,
		command_new_conditional(
			AND_OP,
			command_new_simple(Words({"true"}).to_list(), nullptr),
			command_new_simple(Words({"true"}).to_list(), nullptr)
		),
		command_new_simple(Words({"false"}).to_list(), nullptr)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, ManySimplesAndOrConditional) {
	const char *input = "true && true || false";
	t_command expected = command_new_conditional(
		OR_OP,
		command_new_conditional(
			AND_OP,
			command_new_simple(Words({"true"}).to_list(), nullptr),
			command_new_simple(Words({"true"}).to_list(), nullptr)
		),
		command_new_simple(Words({"false"}).to_list(), nullptr)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, PipelinesAndConditional) {
	const char *input = "true | true && false | true";
	t_command expected = command_new_conditional(
		AND_OP,
		command_new_pipeline(
			command_new_simple(Words({"true"}).to_list(), nullptr),
			command_new_simple(Words({"true"}).to_list(), nullptr)
		),
		command_new_pipeline(
			command_new_simple(Words({"false"}).to_list(), nullptr),
			command_new_simple(Words({"true"}).to_list(), nullptr)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, PipelinesOrConditional) {
	const char *input = "true | false || false | true";
	t_command expected = command_new_conditional(
		OR_OP,
		command_new_pipeline(
			command_new_simple(Words({"true"}).to_list(), nullptr),
			command_new_simple(Words({"false"}).to_list(), nullptr)
		),
		command_new_pipeline(
			command_new_simple(Words({"false"}).to_list(), nullptr),
			command_new_simple(Words({"true"}).to_list(), nullptr)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SimplesAndSubshellsAndConditional) {
	const char *input = "true || (false | true)";
	t_command subshell = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"false"}).to_list(), nullptr),
			command_new_simple(Words({"true"}).to_list(), nullptr)
		),
		nullptr
	);
	t_command expected = command_new_conditional(
		OR_OP,
		command_new_simple(Words({"true"}).to_list(), nullptr),
		subshell
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, SubshellsAndPipelinesConditional) {
	const char *input = "(abc) || thing | (echo hello && world)";
	t_command subshell1 = command_new_subshell(
		command_new_simple(Words({"abc"}).to_list(), nullptr),
		nullptr
	);
	t_command subshell2 = command_new_subshell(
		command_new_conditional(
			AND_OP,
			command_new_simple(Words({"echo", "hello"}).to_list(), nullptr),
			command_new_simple(Words({"world"}).to_list(), nullptr)
		),
		nullptr
	);
	t_command expected = command_new_conditional(
		OR_OP,
		subshell1,
		command_new_pipeline(
			command_new_simple(Words({"thing"}).to_list(), nullptr),
			subshell2
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

TEST(ParserIntegration, AllCommandTypes) {
	const char *input = "(bat cat) << bash || (sh && (dash)) | (ulimit) && idk << im just > saying | stuff";
	t_command bat_cat = command_new_simple(Words({"bat", "cat"}).to_list(), nullptr);
	t_command sh = command_new_simple(Words({"sh"}).to_list(), nullptr);
	t_command dash = command_new_simple(Words({"dash"}).to_list(), nullptr);
	t_command ulimit = command_new_simple(Words({"ulimit"}).to_list(), nullptr);
	t_command stuff = command_new_simple(Words({"stuff"}).to_list(), nullptr);
	t_command idk = command_new_simple(Words({"idk", "just"}).to_list(), Redirections({HereDoc("im"), IntoFile("saying")}).to_list());
	t_command subshell_bat_cat = command_new_subshell(bat_cat, Redirections({HereDoc("bash")}).to_list());
	t_command subshell_ulimit = command_new_subshell(ulimit, nullptr);
	t_command subshell_sh_dash = command_new_subshell(
		command_new_conditional(
			AND_OP, sh, command_new_subshell(dash, nullptr)
		), nullptr
	);

	t_command expected = command_new_conditional(
		AND_OP,
		command_new_conditional(
			OR_OP,
			subshell_bat_cat,
			command_new_pipeline(subshell_sh_dash, subshell_ulimit)
		),
		command_new_pipeline(idk, stuff)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	assert_command_eq(actual, expected);
	command_destroy(actual);
	command_destroy(expected);
}

// ------------------ PARSING ERRORS ------------------

TEST(ParserIntegration, RejectsSubshellWithLeadingRedirections) {
	const char *input = "< infile (hello world)";

	t_command actual;
	ASSERT_EQ(parse(input, &actual), E_UNEXPECTED_TOKEN);
}

TEST(ParserIntegration, RejectsNoMatchingSubshellParen) {
	const char *input = "(abc | (def)";

	t_command actual;
	ASSERT_EQ(parse(input, &actual), E_UNEXPECTED_TOKEN);
}

TEST(ParserIntegration, RejectsUnterminatedPipeline) {
	const char *input = "cat abc | grep def |";

	t_command actual;
	ASSERT_EQ(parse(input, &actual), E_UNEXPECTED_TOKEN);
}

TEST(ParserIntegration, RejectsUnterminatedSubshell) {
	const char *input = "(abc def | ghi";

	t_command actual;
	ASSERT_EQ(parse(input, &actual), E_UNEXPECTED_TOKEN);
}

TEST(ParserIntegration, RejectsRedirectionWithNoFollowingWord) {
	const char *input = "echo hello world < > outfile";

	t_command actual;
	ASSERT_EQ(parse(input, &actual), E_UNEXPECTED_TOKEN);
}

TEST(ParserIntegration, RejectsEmptySubshell) {
	const char *input = "(  )";

	t_command actual;
	ASSERT_EQ(parse(input, &actual), E_UNEXPECTED_TOKEN);
}

TEST(ParserIntegration, RejectsEmptySubshellWithTrailingRedirection) {
	const char *input = "(  ) > outfile";

	t_command actual;
	ASSERT_EQ(parse(input, &actual), E_UNEXPECTED_TOKEN);
}

TEST(ParserIntegration, RejectsWtfWithLeadingRedirectedSubshell) {
	const char *input = "(bat cat) << bash || (sh && (dash)) | << woops (ulimit) && idk << im just > saying | stuff";

	t_command actual;
	ASSERT_EQ(parse(input, &actual), E_UNEXPECTED_TOKEN);
}
