#include "error/t_error.h"
#include "process.h"

#include "libft/ft_io.h"

#include <termios.h>

void reset_tty_properties(int tty_fd, struct termios *tty_properties)
{
	if (tcsetattr(tty_fd, TCSADRAIN, tty_properties) < 0)
		report_error("tcsetattr", "could not reset tty properties");
}
