/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_simple_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:41:55 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:42:46 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./process/process.h"
#include "./t_pid_list/t_pid_list.h"
#include "error/t_error.h"
#include "execute.h"
#include "shell/shell.h"
#include "signal/signal.h"
#include "word/expansions/expand.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define COMMAND_NOT_FOUND_EXIT_CODE 127
#define NOT_EXECUTABLE_EXIT_CODE 126

typedef struct s_execve_variables
{
	char	*path;
	char	**argv;
	char	**envp;
}			t_execve_variables;

static void	execve_or_die(t_state *state,
				t_execve_variables vars) __attribute__((noreturn));

static void	prepare_io_or_die(t_state *state, t_simple *simple, t_io io,
				int fd_to_close);
static void	prepare_execve_vars_or_die(t_state *state, t_simple *simple,
				t_execve_variables *vars);
static void	handle_path_expansion_error(t_state *state,
				t_word_list *command_words, t_error err);

bool		file_is_directory(const char *file_path);

void		free_null_terminated_str_array(char *arr[]);

t_error	launch_simple_command(t_state *state, t_simple *simple, t_io io,
		int fd_to_close)
{
	t_error				err;
	bool				in_child;
	t_execve_variables	variables;

	err = fork_and_push_pid(&in_child, &state->our_children);
	if (err != NO_ERROR)
		return (err);
	if (!in_child)
		return (NO_ERROR);
	else
		pidl_clear(&state->our_children);
	prepare_io_or_die(state, simple, io, fd_to_close);
	if (!simple->words)
		shell_exit(state, EXIT_SUCCESS);
	prepare_execve_vars_or_die(state, simple, &variables);
	reset_signal_handlers();
	execve_or_die(state, variables);
}

static void	prepare_io_or_die(t_state *state, t_simple *simple, t_io io,
		int fd_to_close)
{
	t_error	err;

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
		shell_exit(state, EXIT_FAILURE);
	}
}

static void	prepare_execve_vars_or_die(t_state *state, t_simple *simple,
		t_execve_variables *vars)
{
	t_error	err;

	err = path_expanded_word(state->env, simple->words->contents, &vars->path);
	if (err != NO_ERROR)
		return (handle_path_expansion_error(state, simple->words, err));
	vars->argv = wl_into_word_array(&simple->words);
	if (!vars->argv)
	{
		report_t_error("wl_into_word_array", E_OOM);
		free(vars->path);
		shell_exit(state, EXIT_FAILURE);
	}
	vars->envp = env_make_envp(state->env);
	if (!vars->envp)
	{
		report_t_error("env_make_envp", E_OOM);
		free_null_terminated_str_array(vars->argv);
		free(vars->path);
		shell_exit(state, EXIT_FAILURE);
	}
}

__attribute__((noreturn))
static void	handle_path_expansion_error(t_state *state,
		t_word_list *command_words, t_error err)
{
	if (err == E_COMMAND_NOT_FOUND)
	{
		if (command_words)
			report_error(command_words->contents, "command not found");
		else
			report_error("execute_simple_command", "command not found");
		shell_exit(state, COMMAND_NOT_FOUND_EXIT_CODE);
	}
	else
	{
		report_t_error("path_expanded_word", err);
		shell_exit(state, EXIT_FAILURE);
	}
}

// if (errno == ENOENT) // (execute_cmd.c:5967)
__attribute__((noreturn))
static void	execve_or_die(t_state *state, t_execve_variables vars)
{
	execve(vars.path, vars.argv, vars.envp);
	free_null_terminated_str_array(vars.argv);
	free_null_terminated_str_array(vars.envp);
	if (errno == ENOENT)
		state->last_status = COMMAND_NOT_FOUND_EXIT_CODE;
	else
		state->last_status = NOT_EXECUTABLE_EXIT_CODE;
	if (file_is_directory(vars.path))
	{
		report_error(vars.path, "is a directory");
		free(vars.path);
		shell_exit(state, state->last_status);
	}
	perror("minishell: execve");
	free(vars.path);
	shell_exit(state, state->last_status);
}
