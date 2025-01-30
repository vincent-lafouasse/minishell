/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_error.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:07:44 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:07:45 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h>

/* Update error_repr.c when adding a new variant */
typedef enum e_error
{
	NO_ERROR,
	E_UNRECOGNIZED_TOKEN,
	E_UNEXPECTED_TOKEN,
	E_UNTERMINATED_QUOTE,
	E_OOM,
	E_UNREACHABLE,
	E_NULL_PARAM,
	E_MALFORMED_ENVP,
	E_COMMAND_NOT_FOUND,
	E_FORK,
	E_DUP2,
	E_EXECVE,
	E_ACCESS,
	E_GETCWD,
	E_WRITE,
	E_WAIT,
	E_OPEN,
	E_UNLINK,
	E_PIPE,
	E_DUMMY,
	E_AMBIGUOUS_REDIRECT,
	E_INTERRUPTED,
}			t_error;

const char	*error_repr(t_error err);
bool		is_syscall_related(t_error err);
bool		is_fatal_error(t_error err);
char		parse_error_exit_code(t_error err);

void		report_error_message(const char *reason);
void		report_error(const char *origin, const char *reason);
void		report_syscall_error(const char *syscall);
void		report_t_error(const char *origin, t_error err);
