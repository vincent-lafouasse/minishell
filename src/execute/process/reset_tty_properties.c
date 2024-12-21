#include "process.h"

#include "libft/ft_io.h"

#include <termios.h>

#define TCSETATTR_ERROR "minishell: tcsetattr: could not reset tty properties\n"

void reset_tty_properties(int tty_fd, struct termios *tty_properties)
{
	if (tcsetattr(tty_fd, TCSADRAIN, tty_properties) < 0)
		ft_putendl_fd(TCSETATTR_ERROR, STDERR_FILENO);
}
