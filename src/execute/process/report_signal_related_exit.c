#include "process.h"

#include "word/t_string/t_string.h"

#include "libft/ft_io.h"
#include "libft/ft_string.h"
#include "libft/string.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define LARGEST_POSSIBLE_MESSAGE \
	"minishell: process killed by signal #–2147483648 (core dumped)\n"

static void default_report_signal_related_exit(int signal_id, bool core_dumped)
{
	ft_putstr_fd("minishell: process killed by signal #", STDERR_FILENO);
	ft_putnbr_fd(signal_id, STDERR_FILENO);
	if (core_dumped)
		ft_putstr_fd(" (core dumped)", STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}

void report_signal_related_exit(int status)
{
	t_string	*msg;
	char		*signal_str;
	int			signal_id;
	bool		core_dumped;

	signal_id = WTERMSIG(status);
	core_dumped = WCOREDUMP(status);
	msg = string_new_with_capacity(ft_strlen(LARGEST_POSSIBLE_MESSAGE));
	signal_str = ft_itoa(signal_id);
	if (!signal_str || !msg)
	{
		free(signal_str);
		string_destroy(msg);
		default_report_signal_related_exit(signal_id, core_dumped);
		return;
	}
	string_extend(&msg, "minishell: process killed by signal #");
	string_extend(&msg, signal_str);
	if (core_dumped)
		string_extend(&msg, " (core dumped)");
	string_extend(&msg, "\n");
	write(STDERR_FILENO, msg->data, msg->len);
}
