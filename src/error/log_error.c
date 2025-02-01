/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:07:37 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:09:44 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ft_io.h"
#include "libft/string.h"
#include "t_error.h"
#include "word/t_string/t_string.h"
#include <errno.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

void	report_error_message(const char *reason)
{
	size_t		len;
	t_string	*msg;

	len = ft_strlen(reason) + ft_strlen("minishell: \n");
	msg = string_new_with_capacity(len);
	if (!msg)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(reason, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return ;
	}
	string_extend(&msg, "minishell: ");
	string_extend(&msg, reason);
	string_extend(&msg, "\n");
	write(STDERR_FILENO, msg->data, msg->len);
	string_destroy(msg);
}

// -> prints: "minishell: $ORIGIN: $REASON\n"
void	report_error(const char *origin, const char *reason)
{
	size_t		len;
	t_string	*msg;

	len = ft_strlen(origin) + ft_strlen(reason) + ft_strlen("minishell: : \n");
	msg = string_new_with_capacity(len);
	if (!msg)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(origin, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(reason, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return ;
	}
	string_extend(&msg, "minishell: ");
	string_extend(&msg, origin);
	string_extend(&msg, ": ");
	string_extend(&msg, reason);
	string_extend(&msg, "\n");
	write(STDERR_FILENO, msg->data, msg->len);
	string_destroy(msg);
}

// -> prints: "minishell: $SYSCALL: $STRERROR(ERRNO)\n"
void	report_syscall_error(const char *syscall)
{
	const char	*system_error = strerror(errno);

	report_error(syscall, system_error);
}

// -> prints: "minishell: $REASON:  $ERROR_REPR(err)\n"
void	report_t_error(const char *origin, t_error err)
{
	const char	*reason;

	if (is_syscall_related(err))
		reason = (const char *)strerror(errno);
	else
		reason = error_repr(err);
	report_error(origin, reason);
}
