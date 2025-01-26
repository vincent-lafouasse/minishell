#include "t_error.h"
#include "libft/string.h"
#include "libft/ft_io.h"
#include "word/t_string/t_string.h"

#include <unistd.h>

// -> prints: "minishell: $ORIGIN: $REASON"
void report_error(const char *origin, const char *reason) {
    size_t len = ft_strlen(origin) + ft_strlen(reason) + 15;
    t_string* msg = string_new_with_capacity(len);

    if (!msg) {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(origin, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putstr_fd(reason, STDERR_FILENO);
    }

    string_extend(&msg, "minishell: ");
    string_extend(&msg, origin);
    string_extend(&msg, ": ");
    string_extend(&msg, reason);
    ft_putstr_fd(msg->data, STDERR_FILENO);
}

void report_syscall_error(const char *syscall) {
}

void report_t_error(const char *reason, t_error err) {
}
