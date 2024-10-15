#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "error/t_error.h"
#include "execute/execute.h"
#include "execute/t_env/t_env.h"
#include "log/log.h" // todo remove in production mode
#include "parse/parse.h"
#include "libft/string.h"

#define SHELL_PROMPT "minishell$ "
#define USAGE "./minishell [-c command]"

t_error run_command(const char* input, t_state* state)
{
	t_error err;
	t_command cmd;

	// TODO: double check that bash really does behaves like this
	if (*input == '\0')
	{
		return NO_ERROR;
	}
	add_history(input);

	err = parse(input, &cmd);
	if (err != NO_ERROR)
	{
		return err;
	}

	if (cmd.type == SIMPLE_CMD)
	{
		t_command_result res;
		res = execute_simple_command(state, cmd.simple, (t_io){DO_NOT_PIPE, DO_NOT_PIPE}, NULL);
		log_error(res.error);

		int status;
		int options = 0;
		if (res.pids != NULL)
			waitpid(res.pids->pid, &status, options);
	}
	else if (cmd.type == PIPELINE_CMD)
	{
		t_pid_list* pids = NULL;

		t_command_result res;
		res = execute_pipeline(state, cmd.pipeline, (t_io){DO_NOT_PIPE, DO_NOT_PIPE}, &pids);
		log_error(res.error);
	}
	return NO_ERROR;
}

void run_interpreter(t_state* state)
{
	char		*input;
	t_error err;

	while (1)
	{
		input = readline(SHELL_PROMPT);
		if (!input)
			break; /* eof */
		err = run_command(input, state);
		free(input);
		printf("command status: %s\n", error_repr(err));
	}
	clear_history(); // BAD, it's only here bc i couldnt make it compile with rl_x - poss
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state		state;
	t_error		err;

	err = from_envp((const char **)envp, &state.env);
	if (err != NO_ERROR)
		return EXIT_FAILURE;
	
	if (argc == 1)
		run_interpreter(&state);
	else if (argc == 3 && ft_strncmp(argv[1], "-c", 3) == 0)
		run_command(argv[2], &state);
	else
		printf("%s\n", USAGE);
}
