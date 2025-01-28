#include "execute.h"

#include "error/t_error.h"
#include "signal/signal.h"
#include "word/expansions/expand.h"

#include "./t_pid_list/t_pid_list.h"
#include "./process/process.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#define COMMAND_NOT_FOUND_EXIT_CODE 127
#define NOT_EXECUTABLE_EXIT_CODE 126

bool file_is_directory(const char *file_path);

void shell_cleanup(t_state *);

_Noreturn
static void cleanup_and_die(t_state *state, int with_status)
{
	shell_cleanup(state);
	exit(with_status);
}

static void free_null_terminated_str_array(char *arr[])
{
	size_t i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

_Noreturn
static void execve_and_exit(t_state* state, char* command_path, char **argv, char **envp)
{
	execve(command_path, argv, envp);
	free_null_terminated_str_array(argv);
	free_null_terminated_str_array(envp);

	if (errno == ENOENT) // (execute_cmd.c:5967)
		state->last_status = COMMAND_NOT_FOUND_EXIT_CODE;
	else
		state->last_status = NOT_EXECUTABLE_EXIT_CODE;

	if (file_is_directory(command_path)) { // maybe we should check S_ISREG aswell
		report_error(command_path, "is a directory");
		free(command_path);
		cleanup_and_die(state, state->last_status);
	}

	perror("minishell: execve");
	free(command_path);
	cleanup_and_die(state, state->last_status);
}


_Noreturn
static void simple_cmd_routine(t_state *state, t_simple *simple, t_io io, int fd_to_close)
{
	t_error err;
	char *command_path;

	if (fd_to_close != CLOSE_NOTHING)
		close(fd_to_close);

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("minishell: do_piping: dup2");

	err = apply_redirections(state, simple->redirections);
	if (err != NO_ERROR)
	{
		if (is_syscall_related(err))
			report_syscall_error("apply_redirections");
		else
			report_t_error("apply_redirections", err);
		cleanup_and_die(state, EXIT_FAILURE);
	}

	if (!simple->words) // exit here if command is null
		cleanup_and_die(state, EXIT_SUCCESS);

	err = path_expanded_word(state->env, simple->words->contents, &command_path);
	if (err == E_COMMAND_NOT_FOUND && command_path == NULL)
	{
		report_error(simple->words->contents, "command not found");
		cleanup_and_die(state, COMMAND_NOT_FOUND_EXIT_CODE);
	}
	if (err != NO_ERROR)
	{
		report_t_error("path_expanded_word", err);
		cleanup_and_die(state, EXIT_FAILURE);
	}

	// exit here instead of executing if last_signal is SIGINT
	reset_signal_handlers();

	char** argv = wl_into_word_array(&simple->words);
	if (!argv)
	{
		report_t_error("wl_into_word_array", E_OOM);
		free(command_path);
		cleanup_and_die(state, EXIT_FAILURE);
	}
	char** envp = env_make_envp(state->env);
	if (!envp)
	{
		report_t_error("env_make_envp", E_OOM);
		free_null_terminated_str_array(argv);
		free(command_path);
		cleanup_and_die(state, EXIT_FAILURE);
	}

	execve_and_exit(state, command_path, argv, envp);
}

t_error launch_simple_command(t_state *state, t_simple *simple, t_io io, int fd_to_close)
{
	t_error err;
	bool in_child;

	err = fork_and_push_pid(&in_child, &state->our_children);
	if (err != NO_ERROR)
		return err;

	if (!in_child)
		return NO_ERROR;
	else
		pidl_clear(&state->our_children);

	simple_cmd_routine(state, simple, io, fd_to_close);
}
