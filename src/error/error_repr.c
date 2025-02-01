/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_repr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:07:24 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:51:49 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_error.h"

#define NO_ERROR_REPR "No error"
#define UNRECOGNIZED_TOKEN_REPR "Unrecognized token"
#define UNEXPECTED_TOKEN_REPR "Unexpected token"
#define UNTERMINATED_QUOTE_REPR "Unterminated quote"
#define OUT_OF_MEMORY_REPR "Out of memory"
#define UNREACHABLE_REPR "Unreachable statement"
#define NULL_PARAM_REPR "Received NULL parameter"
#define MALFORMED_ENVP_REPR "Malformed environment"
#define COMMAND_NOT_FOUND_REPR "Command not found"
#define FORK_REPR "Fork error"
#define DUP2_REPR "Dup2 error"
#define ACCESS_REPR "Access error"
#define INTERRUPTED_REPR "Command was interrupted pre-execution"
#define GETCWD_REPR "Getcwd error"
#define WRITE_REPR "Write error"
#define WAIT_REPR "Wait error"
#define OPEN_REPR "Open error"
#define UNLINK_REPR "Unlink error"
#define PIPE_REPR "Pipe error"
#define AMBIGUOUS_REDIRECT_REPR "Ambiguous redirection"

#define UNKNOWN_ERROR_REPR "Unknown error"

const char	*error_repr_continued(t_error err);

const char	*error_repr(t_error err)
{
	if (err == NO_ERROR)
		return (NO_ERROR_REPR);
	if (err == E_UNRECOGNIZED_TOKEN)
		return (UNRECOGNIZED_TOKEN_REPR);
	if (err == E_UNEXPECTED_TOKEN)
		return (UNEXPECTED_TOKEN_REPR);
	if (err == E_UNTERMINATED_QUOTE)
		return (UNTERMINATED_QUOTE_REPR);
	if (err == E_OOM)
		return (OUT_OF_MEMORY_REPR);
	if (err == E_UNREACHABLE)
		return (UNREACHABLE_REPR);
	if (err == E_NULL_PARAM)
		return (NULL_PARAM_REPR);
	if (err == E_MALFORMED_ENVP)
		return (MALFORMED_ENVP_REPR);
	if (err == E_COMMAND_NOT_FOUND)
		return (COMMAND_NOT_FOUND_REPR);
	if (err == E_FORK)
		return (FORK_REPR);
	if (err == E_DUP2)
		return (DUP2_REPR);
	return (error_repr_continued(err));
}

const char	*error_repr_continued(t_error err)
{
	if (err == E_ACCESS)
		return (ACCESS_REPR);
	if (err == E_INTERRUPTED)
		return (INTERRUPTED_REPR);
	if (err == E_GETCWD)
		return (GETCWD_REPR);
	if (err == E_WRITE)
		return (WRITE_REPR);
	if (err == E_WAIT)
		return (WAIT_REPR);
	if (err == E_OPEN)
		return (OPEN_REPR);
	if (err == E_UNLINK)
		return (UNLINK_REPR);
	if (err == E_PIPE)
		return (PIPE_REPR);
	if (err == E_AMBIGUOUS_REDIRECT)
		return (AMBIGUOUS_REDIRECT_REPR);
	return (UNKNOWN_ERROR_REPR);
}
