#include "process.h"

#include "libft/ft_io.h"

#include <unistd.h>
#include <sys/wait.h>

void report_signal_related_exit(int status)
{
	int signal_id;

	signal_id = WTERMSIG(status);
	ft_putstr_fd("minishell: process killed by signal #", STDERR_FILENO);
	ft_putnbr_fd(signal_id, STDERR_FILENO);
	if (WCOREDUMP(status))
		ft_putstr_fd(" (core dumped)", STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}
