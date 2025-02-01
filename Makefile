NAME = minishell

SRCS_PATH = src
SRCS = $(SRCS_PATH)/main.c \
	$(SRCS_PATH)/interpreter.c \
	$(SRCS_PATH)/shell_init.c \
	$(SRCS_PATH)/shell/cleanup.c \
	$(SRCS_PATH)/signal/readline.c \
	$(SRCS_PATH)/signal/handlers.c \
	$(SRCS_PATH)/signal/noninteractive_readline.c \
	$(SRCS_PATH)/error/parse_error_exit_code.c \
	$(SRCS_PATH)/error/is_syscall_related.c \
	$(SRCS_PATH)/error/log_error.c \
	$(SRCS_PATH)/error/error_repr.c \
	$(SRCS_PATH)/error/is_fatal_error.c \
	$(SRCS_PATH)/parse/parse.c \
	$(SRCS_PATH)/parse/t_parser/t_parser.c \
	$(SRCS_PATH)/parse/t_parser/parser_push.c \
	$(SRCS_PATH)/parse/t_symbol/t_symbol.c \
	$(SRCS_PATH)/parse/tokenize/tokenize.c \
	$(SRCS_PATH)/parse/tokenize/t_token.c \
	$(SRCS_PATH)/parse/tokenize/token_repr.c \
	$(SRCS_PATH)/parse/tokenize/t_lexer/lexer_scan_metachar_operator.c \
	$(SRCS_PATH)/parse/tokenize/t_lexer/lexer_skip_whitespace.c \
	$(SRCS_PATH)/parse/tokenize/t_lexer/fill_token.c \
	$(SRCS_PATH)/parse/tokenize/t_lexer/t_lexer_init.c \
	$(SRCS_PATH)/parse/tokenize/t_lexer/lexer_scan_next_token.c \
	$(SRCS_PATH)/parse/tokenize/t_lexer/lexer_scan_word.c \
	$(SRCS_PATH)/parse/tokenize/t_lexer/lexer_advance_peek.c \
	$(SRCS_PATH)/parse/tokenize/t_token_list/tkl_clear.c \
	$(SRCS_PATH)/parse/tokenize/t_token_list/tkl_delone.c \
	$(SRCS_PATH)/parse/tokenize/t_token_list/tkl_push_back.c \
	$(SRCS_PATH)/parse/tokenize/t_token_list/tkl_new.c \
	$(SRCS_PATH)/parse/productions/produce_simple_command.c \
	$(SRCS_PATH)/parse/productions/produce_io_redirect.c \
	$(SRCS_PATH)/parse/productions/produce_redirect_list_rest.c \
	$(SRCS_PATH)/parse/productions/produce_complete_command_rest.c \
	$(SRCS_PATH)/parse/productions/produce_io_file.c \
	$(SRCS_PATH)/parse/productions/produce_program.c \
	$(SRCS_PATH)/parse/productions/produce_command.c \
	$(SRCS_PATH)/parse/productions/produce_cmd_suffix_rest.c \
	$(SRCS_PATH)/parse/productions/produce_subshell.c \
	$(SRCS_PATH)/parse/productions/produce_subshell_precedes.c \
	$(SRCS_PATH)/parse/productions/produce_redirect_list.c \
	$(SRCS_PATH)/parse/productions/produce_complete_command.c \
	$(SRCS_PATH)/parse/productions/produce_pipeline_rest.c \
	$(SRCS_PATH)/parse/productions/produce_cmd_prefix_precedes.c \
	$(SRCS_PATH)/parse/productions/produce_cmd_prefix_rest.c \
	$(SRCS_PATH)/parse/productions/produce_cmd_suffix.c \
	$(SRCS_PATH)/parse/productions/produce_cmd_prefix.c \
	$(SRCS_PATH)/parse/productions/produce_here_end.c \
	$(SRCS_PATH)/parse/productions/produce_pipeline.c \
	$(SRCS_PATH)/parse/productions/produce_filename.c \
	$(SRCS_PATH)/parse/productions/produce_io_here.c \
	$(SRCS_PATH)/parse/productions/produce_maybe_cmd_suffix.c \
	$(SRCS_PATH)/parse/reduction/reduce_simple_command.c \
	$(SRCS_PATH)/parse/reduction/reduce_simple_command_like.c \
	$(SRCS_PATH)/parse/reduction/gather_leaves.c \
	$(SRCS_PATH)/parse/reduction/reduce_complete_command.c \
	$(SRCS_PATH)/parse/reduction/reduce_pipeline.c \
	$(SRCS_PATH)/parse/reduction/reduction.c \
	$(SRCS_PATH)/parse/reduction/conditional_command_data.c \
	$(SRCS_PATH)/parse/reduction/invert_conditional_associativity.c \
	$(SRCS_PATH)/parse/reduction/reduce_command.c \
	$(SRCS_PATH)/parse/reduction/reduce_subshell.c \
	$(SRCS_PATH)/parse/t_command/conditional_command_constructor.c \
	$(SRCS_PATH)/parse/t_command/properties.c \
	$(SRCS_PATH)/parse/t_command/subshell_constructor.c \
	$(SRCS_PATH)/parse/t_command/command_destroy.c \
	$(SRCS_PATH)/parse/t_command/simple_command_constructor.c \
	$(SRCS_PATH)/parse/t_command/pipeline_constructor.c \
	$(SRCS_PATH)/parse/t_command/empty.c \
	$(SRCS_PATH)/parse/t_command/destructors.c \
	$(SRCS_PATH)/word/word_clone.c \
	$(SRCS_PATH)/word/truncate_to_one_line_if_necessary.c \
	$(SRCS_PATH)/word/t_word_list/destroy.c \
	$(SRCS_PATH)/word/t_word_list/push.c \
	$(SRCS_PATH)/word/t_word_list/t_word_list.c \
	$(SRCS_PATH)/word/expansions/remove_outer_quotes.c \
	$(SRCS_PATH)/word/expansions/expand_variable.c \
	$(SRCS_PATH)/word/expansions/quote_removed_word.c \
	$(SRCS_PATH)/word/expansions/t_word_quotes_list.c \
	$(SRCS_PATH)/word/expansions/make_joined.c \
	$(SRCS_PATH)/word/expansions/parse.c \
	$(SRCS_PATH)/word/expansions/wql_push.c \
	$(SRCS_PATH)/word/expansions/pathname/expand_internals.c \
	$(SRCS_PATH)/word/expansions/pathname/expand.c \
	$(SRCS_PATH)/word/expansions/expand.c \
	$(SRCS_PATH)/word/t_string/c_string.c \
	$(SRCS_PATH)/word/t_string/insert.c \
	$(SRCS_PATH)/word/t_string/alloc.c \
	$(SRCS_PATH)/word/t_string/destructor.c \
	$(SRCS_PATH)/word/t_string/realloc.c \
	$(SRCS_PATH)/word/t_string/constructor.c \
	$(SRCS_PATH)/redirection/t_io/operations.c \
	$(SRCS_PATH)/redirection/t_io/constructor.c \
	$(SRCS_PATH)/redirection/t_redir_list/push.c \
	$(SRCS_PATH)/redirection/t_redir_list/apply_redirections.c \
	$(SRCS_PATH)/redirection/t_redir_list/redir_kind.c \
	$(SRCS_PATH)/redirection/t_redir_list/t_redir_list.c \
	$(SRCS_PATH)/redirection/t_redir_list/here_document.c \
	$(SRCS_PATH)/redirection/t_redir_list/redirection_repr.c \
	$(SRCS_PATH)/execute/execute.c \
	$(SRCS_PATH)/execute/simple.c \
	$(SRCS_PATH)/execute/subshell.c \
	$(SRCS_PATH)/execute/pipeline.c \
	$(SRCS_PATH)/execute/conditional.c \
	$(SRCS_PATH)/execute/file_is_directory.c \
	$(SRCS_PATH)/execute/launch_simple_command_utils.c \
	$(SRCS_PATH)/execute/launch_simple_command.c \
	$(SRCS_PATH)/execute/t_pid_list/pop.c \
	$(SRCS_PATH)/execute/t_pid_list/push.c \
	$(SRCS_PATH)/execute/t_pid_list/t_pid_list.c \
	$(SRCS_PATH)/execute/t_env/t_env.c \
	$(SRCS_PATH)/execute/t_env/constructor.c \
	$(SRCS_PATH)/execute/t_env/destructor.c \
	$(SRCS_PATH)/execute/t_env/getter.c \
	$(SRCS_PATH)/execute/t_env/push.c \
	$(SRCS_PATH)/execute/t_env/t_env_internals.c \
	$(SRCS_PATH)/execute/builtin/unset.c \
	$(SRCS_PATH)/execute/builtin/env.c \
	$(SRCS_PATH)/execute/builtin/pwd.c \
	$(SRCS_PATH)/execute/builtin/builtin.c \
	$(SRCS_PATH)/execute/builtin/exit/exit.c \
	$(SRCS_PATH)/execute/builtin/exit/checked_atoi.c \
	$(SRCS_PATH)/execute/builtin/exit/checked_atou.c \
	$(SRCS_PATH)/execute/builtin/cd.c \
	$(SRCS_PATH)/execute/builtin/export/do_assignments.c \
	$(SRCS_PATH)/execute/builtin/export/print_all_variables.c \
	$(SRCS_PATH)/execute/builtin/export/export.c \
	$(SRCS_PATH)/execute/builtin/echo.c \
	$(SRCS_PATH)/execute/process/fork.c \
	$(SRCS_PATH)/execute/process/wait_for_process.c \
	$(SRCS_PATH)/execute/process/wait_for_pipeline.c \
	$(SRCS_PATH)/execute/process/report_signal_related_exit.c \
	$(SRCS_PATH)/execute/process/kill.c \
	$(SRCS_PATH)/execute/process/reset_tty_properties.c \
	$(SRCS_PATH)/execute/process/get_exit_status.c \
	$(SRCS_PATH)/execute/process/wait_through_signals.c

CC ?= cc
CFLAGS += -Wall -Wextra -g3
# CFLAGS += -Werror
CPPFLAGS += -Isrc -MMD -MP
LDFLAGS += -lreadline

OBJS := $(SRCS:%=build/%.o)
DEPS := $(OBJS:.o=.d)

LIBFT_PATH = ./lib/libft
LIBFT = $(LIBFT_PATH)/libft.a
LIBFT_INCLUDE_DIR = $(LIBFT_PATH)/include
LIBFT_MAKE_DIR = $(LIBFT_PATH)

CPPFLAGS += -I$(LIBFT_INCLUDE_DIR)
LDFLAGS += $(LIBFT)

LIBS = $(LIBFT)

.PHONY: all
all: build

.PHONY: bonus
bonus: build

.PHONY: build
build: $(NAME)

.PHONY: run
run: build
	./$(NAME)

# linking stage
$(NAME): $(OBJS) $(LIBS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

# compiling stage
build/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_MAKE_DIR)

.PHONY: re
re: fclean build

.PHONY: clean
clean:
	rm -rf build/src

.PHONY: fclean
fclean: clean
	rm -rf build
	rm -rf $(NAME)

.PHONY: test
test: $(LIBS)
	cmake -DCMAKE_BUILD_TYPE=DEBUG -S test -B build/test
	cmake --build build/test
	GTEST_COLOR=1 ctest --test-dir build/test $(CTEST_OPT)

.PHONY: vtest
ifneq ($(TEST_WITH_MEMCHECK),)
vtest: CTEST_OPT += -T memcheck
endif
vtest: CTEST_OPT += -V
vtest: test

.PHONY: exec_test
exec_test: build
	bash  test/src/execution_test/main.sh


.PHONY: test_libft
test_libft: $(LIBFT)
	cmake -S lib/libft/test -B build/test_libft
	cmake --build build/test_libft
	GTEST_COLOR=1 ctest --test-dir build/test_libft -V

.PHONY: run_memcheck
run_memcheck: SUPPRESSIONS_FILE = aux/rl_leaks.supp
run_memcheck: VALGRIND_FLAGS += --leak-check=full
run_memcheck: VALGRIND_FLAGS += --show-leak-kinds=all
run_memcheck: VALGRIND_FLAGS += --track-origins=yes
run_memcheck: VALGRIND_FLAGS += --suppressions=$(SUPPRESSIONS_FILE)
run_memcheck: VALGRIND_FLAGS += --trace-children=yes
run_memcheck: build
	valgrind $(VALGRIND_FLAGS) ./$(NAME)

.PHONY: update
update: clean
	mkdir -p build
	bear  --output build/compile_commands.json -- make build

# aliases
.PHONY: b c u r rm t vt xt
b: build
c: clean
u: update
r: run
rm: run_memcheck
t: test
vt: vtest
xt: exec_test

-include $(DEPS)
