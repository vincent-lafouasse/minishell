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
	t_command expected = command_new_simple(
		Words({"echo", "hello", "world"}).get(),
		Redirections({AppendIntoFile("appendfile")}).get()
	);

    t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndLeadingHereDocument) {
		const char* input = "<< eof echo hello world";
		t_command expected = command_new_simple(
				Words({"echo", "hello", "world"}).get(),
				Redirections({HereDoc("eof")}).get()
		);

		t_command actual;
		ASSERT_EQ(parse(input, &actual), NO_ERROR);
		ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsAndAllLeadingRedirections) {
		const char* input = "> outfile < infile >> appendfile << eof echo hello world";
		t_command expected = command_new_simple(
				Words({"echo", "hello", "world"}).get(),
				Redirections({
						IntoFile("outfile"),
						FromFile("infile"),
						AppendIntoFile("appendfile"),
						HereDoc("eof"),
				}).get()
		);

		t_command actual;
		ASSERT_EQ(parse(input, &actual), NO_ERROR);
		ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleWordsMixedWithAllLeadingRedirections) {
		const char* input = "> outfile echo < infile hello >> appendfile world << eof";
		t_command expected = command_new_simple(
				Words({"echo", "hello", "world"}).get(),
				Redirections({
						IntoFile("outfile"),
						FromFile("infile"),
						AppendIntoFile("appendfile"),
						HereDoc("eof"),
				}).get()
		);

		t_command actual;
		ASSERT_EQ(parse(input, &actual), NO_ERROR);
		ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleRedirection) {
		const char* input = "< infile";
		t_command expected = command_new_simple(
				nullptr,
				Redirections({
						FromFile("infile"),
				}).get()
		);

		t_command actual;
		ASSERT_EQ(parse(input, &actual), NO_ERROR);
		ASSERT_TRUE(command_eq(actual, expected));
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
				}).get()
		);

		t_command actual;
		ASSERT_EQ(parse(input, &actual), NO_ERROR);
		ASSERT_TRUE(command_eq(actual, expected));
}

// ------------------ PIPELINES ------------------

TEST(ParserIntegration, SimplePipeline) {
	const char *input = "cat /etc/passwd | sort";
	t_command actual;
	t_command expected;
	t_error err;

	err = parse(input, &actual);
	ASSERT_EQ(err, NO_ERROR);

	expected = command_new_pipeline(
		command_new_simple(Words({"cat", "/etc/passwd"}).get(), nullptr),
		command_new_simple(Words({"sort"}).get(), nullptr)
	);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, MultiPipeline) {
	const char *input = "cat /etc/passwd | sort | head -n3";
	t_command actual;
	t_command expected;
	t_error err;

	err = parse(input, &actual);
	ASSERT_EQ(err, NO_ERROR);

	expected = command_new_pipeline(
		command_new_simple(Words({"cat", "/etc/passwd"}).get(), nullptr),
		command_new_pipeline(
			command_new_simple(Words({"sort"}).get(), nullptr),
			command_new_simple(Words({"head", "-n3"}).get(), nullptr)
		)
	);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, PipelineAndRedirections) {
	const char *input = "cat /etc/passwd | < Makefile sort | head -n3 > outfile";
	t_command actual;
	t_command expected;
	t_error err;

	err = parse(input, &actual);
	ASSERT_EQ(err, NO_ERROR);

	expected = command_new_pipeline(
		command_new_simple(Words({"cat", "/etc/passwd"}).get(), nullptr),
		command_new_pipeline(
			command_new_simple(Words({"sort"}).get(), Redirections({FromFile("Makefile")}).get()),
			command_new_simple(Words({"head", "-n3"}).get(), Redirections({IntoFile("outfile")}).get())
		)
	);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, PipelineWithTrailingLeadingRedirections) {
	const char *input = "< infile sort | head -n3 > outfile";
	t_command actual;
	t_command expected;
	t_error err;

	err = parse(input, &actual);
	ASSERT_EQ(err, NO_ERROR);

	expected = command_new_pipeline(
		command_new_simple(Words({"sort"}).get(), Redirections({FromFile("infile")}).get()),
		command_new_simple(Words({"head", "-n3"}).get(), Redirections({IntoFile("outfile")}).get())
	);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, MultiPipelineWithTrailingLeadingRedirections) {
	const char *input = "< infile sort | grep c | cut -d: | head > outfile";
	t_command actual;
	t_command expected;
	t_error err;

	err = parse(input, &actual);
	ASSERT_EQ(err, NO_ERROR);

	expected = command_new_pipeline(
		command_new_simple(Words({"sort"}).get(), Redirections({FromFile("infile")}).get()),
		command_new_pipeline(
			command_new_simple(Words({"grep", "c"}).get(), nullptr),
			command_new_pipeline(
				command_new_simple(Words({"cut", "-d:"}).get(), nullptr),
				command_new_simple(Words({"head"}).get(), Redirections({IntoFile("outfile")}).get())
			)
		)
	);
	ASSERT_TRUE(command_eq(actual, expected));
}

// ------------------ SUBSHELLS ------------------

TEST(ParserIntegration, SimpleSubshell) {
	const char *input = "(echo hello world)";
	t_command actual;
	t_command expected;
	t_error err;

	err = parse(input, &actual);
	ASSERT_EQ(err, NO_ERROR);

	expected = command_new_subshell(
		command_new_simple(Words({"echo", "hello", "world"}).get(), nullptr),
		nullptr
	);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SubshellWithPipeline) {
	const char *input = "(cat /etc/passwd | sort)";
	t_command expected = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"cat", "/etc/passwd"}).get(), nullptr),
			command_new_simple(Words({"sort"}).get(), nullptr)
		),
		nullptr
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SubshellWithTrailingRedirection) {
	const char *input = "(cat /etc/passwd | sort) > outfile";
	t_command actual;
	t_command expected;
	t_error err;

	err = parse(input, &actual);
	ASSERT_EQ(err, NO_ERROR);

	expected = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"cat", "/etc/passwd"}).get(), nullptr),
			command_new_simple(Words({"sort"}).get(), nullptr)
		),
		Redirections({IntoFile("outfile")}).get()
	);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SubshellWithAllTrailingRedirections) {
	const char *input = "(cat /etc/passwd | sort) > outfile < infile >> appendfile";
	t_command expected = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"cat", "/etc/passwd"}).get(), nullptr),
			command_new_simple(Words({"sort"}).get(), nullptr)
		),
		Redirections({IntoFile("outfile"), FromFile("infile"), AppendIntoFile("appendfile")}).get()
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SubshellInPipeline) {
	const char *input = "cat /etc/passwd | (sort | grep abc) | wc";
	t_command subshell = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"sort"}).get(), nullptr),
			command_new_simple(Words({"grep", "abc"}).get(), nullptr)
		),
		nullptr
	);
	t_command expected = command_new_pipeline(
		command_new_simple(Words({"cat", "/etc/passwd"}).get(), nullptr),
		command_new_pipeline(
			subshell,
			command_new_simple(Words({"wc"}).get(), nullptr)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleNestedSubshellPipeline) {
	const char *input = "((cat /etc/passwd | grep a) | sort | grep abc)";
	t_command subshell = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"cat", "/etc/passwd"}).get(), nullptr),
			command_new_simple(Words({"grep", "a"}).get(), nullptr)
		),
		nullptr
	);
	t_command pipeline = command_new_pipeline(
		subshell,
		command_new_pipeline(
			command_new_simple(Words({"sort"}).get(), nullptr),
			command_new_simple(Words({"grep", "abc"}).get(), nullptr)
		)
	);
	t_command expected = command_new_subshell(pipeline, nullptr);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, NestedSubshellPipelines) {
	const char *input = "(a | (b | (c | (d | (e | (f | g))))))";
	t_command fg = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"f"}).get(), nullptr),
			command_new_simple(Words({"g"}).get(), nullptr)
		),
		nullptr
	);
	t_command efg = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"e"}).get(), nullptr),
			fg
		),
		nullptr
	);
	t_command defg = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"d"}).get(), nullptr),
			efg
		),
		nullptr
	);
	t_command cdefg = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"c"}).get(), nullptr),
			defg
		),
		nullptr
	);
	t_command bcdefg = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"b"}).get(), nullptr),
			cdefg
		),
		nullptr
	);
	t_command expected = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"a"}).get(), nullptr),
			bcdefg
		),
		nullptr
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

// ------------------ CONDITIONALS ------------------

TEST(ParserIntegration, SimpleCommandOrConditional) {
	const char *input = "false || true";
	t_command expected = command_new_conditional(
		OR_OP,
		command_new_simple(Words({"false"}).get(), nullptr),
		command_new_simple(Words({"true"}).get(), nullptr)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimpleCommandAndConditional) {
	const char *input = "true && true";
	t_command expected = command_new_conditional(
		AND_OP,
		command_new_simple(Words({"true"}).get(), nullptr),
		command_new_simple(Words({"true"}).get(), nullptr)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, ManySimplesOrConditional) {
	const char *input = "false || false || true";
	t_command expected = command_new_conditional(
		OR_OP,
		command_new_simple(Words({"false"}).get(), nullptr),
		command_new_conditional(
			OR_OP,
			command_new_simple(Words({"false"}).get(), nullptr),
			command_new_simple(Words({"true"}).get(), nullptr)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, ManySimplesAndConditional) {
	const char *input = "true && true && false";
	t_command expected = command_new_conditional(
		AND_OP,
		command_new_simple(Words({"true"}).get(), nullptr),
		command_new_conditional(
			AND_OP,
			command_new_simple(Words({"true"}).get(), nullptr),
			command_new_simple(Words({"false"}).get(), nullptr)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, ManySimplesAndOrConditional) {
	const char *input = "true && true || false";
	t_command expected = command_new_conditional(
		AND_OP,
		command_new_simple(Words({"true"}).get(), nullptr),
		command_new_conditional(
			OR_OP,
			command_new_simple(Words({"true"}).get(), nullptr),
			command_new_simple(Words({"false"}).get(), nullptr)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, PipelinesAndConditional) {
	const char *input = "true | true && false | true";
	t_command expected = command_new_conditional(
		AND_OP,
		command_new_pipeline(
			command_new_simple(Words({"true"}).get(), nullptr),
			command_new_simple(Words({"true"}).get(), nullptr)
		),
		command_new_pipeline(
			command_new_simple(Words({"false"}).get(), nullptr),
			command_new_simple(Words({"true"}).get(), nullptr)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, PipelinesOrConditional) {
	const char *input = "true | false || false | true";
	t_command expected = command_new_conditional(
		OR_OP,
		command_new_pipeline(
			command_new_simple(Words({"true"}).get(), nullptr),
			command_new_simple(Words({"false"}).get(), nullptr)
		),
		command_new_pipeline(
			command_new_simple(Words({"false"}).get(), nullptr),
			command_new_simple(Words({"true"}).get(), nullptr)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SimplesAndSubshellsAndConditional) {
	const char *input = "true || (false | true)";
	t_command subshell = command_new_subshell(
		command_new_pipeline(
			command_new_simple(Words({"false"}).get(), nullptr),
			command_new_simple(Words({"true"}).get(), nullptr)
		),
		nullptr
	);
	t_command expected = command_new_conditional(
		OR_OP,
		command_new_simple(Words({"true"}).get(), nullptr),
		subshell
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, SubshellsAndPipelinesConditional) {
	const char *input = "(abc) || thing | (echo hello && world)";
	t_command subshell1 = command_new_subshell(
		command_new_simple(Words({"abc"}).get(), nullptr),
		nullptr
	);
	t_command subshell2 = command_new_subshell(
		command_new_conditional(
			AND_OP,
			command_new_simple(Words({"echo", "hello"}).get(), nullptr),
			command_new_simple(Words({"world"}).get(), nullptr)
		),
		nullptr
	);
	t_command expected = command_new_conditional(
		OR_OP,
		subshell1,
		command_new_pipeline(
			command_new_simple(Words({"thing"}).get(), nullptr),
			subshell2
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
}

TEST(ParserIntegration, AllCommandTypes) {
	const char *input = "(bat cat) << bash || (sh && (dash)) | (ulimit) && idk << im just > saying | stuff";
	t_command bat_cat = command_new_simple(Words({"bat", "cat"}).get(), nullptr);
	t_command sh = command_new_simple(Words({"sh"}).get(), nullptr);
	t_command dash = command_new_simple(Words({"dash"}).get(), nullptr);
	t_command ulimit = command_new_simple(Words({"ulimit"}).get(), nullptr);
	t_command stuff = command_new_simple(Words({"stuff"}).get(), nullptr);
	t_command idk = command_new_simple(Words({"idk", "just"}).get(), Redirections({HereDoc("im"), IntoFile("saying")}).get());
	t_command subshell_bat_cat = command_new_subshell(bat_cat, Redirections({HereDoc("bash")}).get());
	t_command subshell_ulimit = command_new_subshell(ulimit, nullptr);
	t_command subshell_sh_dash = command_new_subshell(
		command_new_conditional(
			AND_OP, sh, command_new_subshell(dash, nullptr)
		), nullptr
	);

	t_command expected = command_new_conditional(
		OR_OP,
		subshell_bat_cat,
		command_new_conditional(
			AND_OP,
			command_new_pipeline(subshell_sh_dash, subshell_ulimit),
			command_new_pipeline(idk, stuff)
		)
	);

	t_command actual;
	ASSERT_EQ(parse(input, &actual), NO_ERROR);
	ASSERT_TRUE(command_eq(actual, expected));
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
