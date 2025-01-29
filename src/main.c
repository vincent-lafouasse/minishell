#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <assert.h> // temporarily, run_command

#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

#include "error/t_error.h"
#include "shell/shell.h"
#include "execute/execute.h"
#include "execute/t_env/t_env.h"
#include "io/t_io/t_io.h"
#include "log/log.h" // todo remove in production mode
#include "parse/parse.h"
#include "libft/string.h"
#include "libft/ft_io.h"
#include "parse/t_command/t_command.h"
#include "signal/signal.h"
#include "io/t_redir_list/t_redir_list.h"

#define SHELL_PROMPT "minishell$ "
#define USAGE "./minishell [-c command]"

t_error run_and_parse_command(const char* input, t_state* state)
{
	t_error err;

	err = parse(input, &state->root);
	if (err != NO_ERROR)
	{
		state->last_status = parse_error_exit_code(err);
		return err;
	}

	if (!command_is_initialized(state->root))
		return NO_ERROR;

	if (last_signal == SIGINT)
	{
		state->last_status = 128 + SIGINT;
		command_destroy_and_clear(&state->root);
		return E_INTERRUPTED;
	}

	err = gather_here_documents(state->root);
	if (err != NO_ERROR)
	{
		command_destroy_and_clear(&state->root);
		return (err);
	}

	t_command_result res = execute_command(state, state->root);
	command_destroy_and_clear(&state->root);
	state->last_status = res.status_code;
	return res.error;
}

void truncate_to_one_line_if_necessary(char *input)
{
	char *line_break;

	line_break = ft_strchr(input, '\n');
	if (line_break)
		*line_break = '\0';
}

char *interactive_read_line(t_state *state)
{
	char *input;

	while (1)
	{
		last_signal = 0;
		input = readline(SHELL_PROMPT);
		if (input == NULL)
			return (NULL);
		if (last_signal != SIGINT)
			break;
		free(input); /* we've caught a C-c signal; repeat */
		state->last_status = 128 + last_signal;
	}

	if (*input != '\0')
		add_history(input);

	truncate_to_one_line_if_necessary(input);
	return (input);
}

void run_interpreter(t_state* state)
{
	t_error err;

	err = NO_ERROR;
	while (err != E_OOM)
	{
		install_interactive_handlers();
		state->line = interactive_read_line(state);
		if (!state->line)
			break; /* eof or read error */
		//install_execution_handlers();

		err = run_and_parse_command(state->line, state);
		if (err != NO_ERROR)
			report_error_message(error_repr(err));
		free(state->line);
		state->line = NULL;
	}
	rl_clear_history();
	if (err != E_OOM)
		ft_putstr_fd("exit\n", STDERR_FILENO);
}

char *non_interactive_read_line(t_state *state)
{
	char *input;

	input = readline(NULL);
	if (last_signal == SIGINT || input == NULL)
	{
		if (last_signal == SIGINT)
			state->last_status = 128 + SIGINT;
		free(input);
		return (NULL);
	}
	truncate_to_one_line_if_necessary(input);
	return (input);
}

void run_non_interactive_loop(t_state *state)
{
	t_error err;

	err = NO_ERROR;
	while (err != E_OOM && err != E_INTERRUPTED)
	{
		install_non_interactive_handlers();
		state->line = non_interactive_read_line(state);
		if (!state->line)
			break; /* no more bytes to read on stdin or read error */
		//install_execution_handlers();

		err = run_and_parse_command(state->line, state);
		if (err != NO_ERROR)
			report_error_message(error_repr(err));
		free(state->line);
		state->line = NULL;
	}
}

void init_interactive(t_state *state_out)
{
	int ret;

	ret = tcgetattr(STDERR_FILENO, &state_out->tty_properties);
	state_out->tty_properties_initialized = ret == 0;
	state_out->is_interactive = true;
}

#define DEFAULT_PATH_VALUE											\
  "/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin:."

static t_error add_if_not_set(t_env **env, const char *key, const char *value)
{
	if (env_key_exists(*env, key))
		return (NO_ERROR);
	return env_insert(env, key, value);
}

t_error set_up_environment(t_env **env, char *envp[])
{
	t_error err;

	err = from_envp(envp, env);
	if (err != NO_ERROR)
		return (err);
	err = add_if_not_set(env, "TERM", "dumb");
	if (err != NO_ERROR)
	{
		env_destroy(env);
		return (NO_ERROR);
	}
	err = add_if_not_set(env, "PATH", DEFAULT_PATH_VALUE);
	if (err != NO_ERROR)
	{
		env_destroy(env);
		return (NO_ERROR);
	}
	return (NO_ERROR);
}

t_error shell_init(char *envp[], bool dash_c, t_state *state_out)
{
	t_error		err;

	rl_outstream = stderr;
	*state_out = (t_state){0};
	err = set_up_environment(&state_out->env, envp);
	if (err != NO_ERROR)
	{
		report_t_error("set_up_environment", err);
		return (err);
	}
	if (!dash_c && isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		init_interactive(state_out);
	else
		state_out->is_interactive = false;
	return (NO_ERROR);
}

void shell_cleanup(t_state *state)
{
	command_destroy_and_clear(&state->root);
	free(state->line);
	rl_clear_history();
	env_destroy(&state->env);
	pidl_clear(&state->our_children);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state		state;
	t_error		err;

	bool executing_arg_command = argc == 3 && ft_strncmp(argv[1], "-c", 3) == 0;
	err = shell_init(envp, executing_arg_command, &state);
	if (err != NO_ERROR)
		return EXIT_FAILURE;
	
	if (argc == 1 && state.is_interactive)
		run_interpreter(&state);
	else if (argc == 1 && !state.is_interactive)
		run_non_interactive_loop(&state);
	else if (executing_arg_command)
	{
		state.is_interactive = false;
		truncate_to_one_line_if_necessary(argv[2]);
		err = run_and_parse_command(argv[2], &state);
		if (err != NO_ERROR)
			report_error_message(error_repr(err));
	}
	else
		printf("%s\n", USAGE);
	shell_cleanup(&state);
	exit(state.last_status);
}
