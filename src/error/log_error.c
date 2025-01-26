#include "t_error.h"
#include "libft/ft_io.h"
#include "libft/string.h"
#include "word/t_string/t_string.h"

#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

// -> prints: "minishell: $ORIGIN: $REASON\n"
void report_error(const char *origin, const char *reason)
{
    size_t len = ft_strlen(origin) + ft_strlen(reason) + ft_strlen("minishell: : \n");
    t_string *msg = string_new_with_capacity(len);

    if (!msg)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(origin, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putstr_fd(reason, STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
    }

    string_extend(&msg, "minishell: ");
    string_extend(&msg, origin);
    string_extend(&msg, ": ");
    string_extend(&msg, reason);
    string_extend(&msg, "\n");
    write(STDERR_FILENO, msg->data, msg->len);
}

// -> prints: "minishell: $SYSCALL: $STRERROR(ERRNO)\n"
void report_syscall_error(const char *syscall)
{
    const char *system_error = strerror(errno);
    size_t len = ft_strlen(syscall) + ft_strlen(system_error) +
                 ft_strlen("minishell: : \n");
    t_string *msg = string_new_with_capacity(len);

    if (!msg)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(syscall, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putstr_fd(system_error, STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
    }

    string_extend(&msg, "minishell: ");
    string_extend(&msg, syscall);
    string_extend(&msg, ": ");
    string_extend(&msg, system_error);
    string_extend(&msg, "\n");
    write(STDERR_FILENO, msg->data, msg->len);
}

// -> prints: "minishell: $REASON:  $ERROR_REPR(err)\n"
void report_t_error(const char *reason, t_error err)
{
    const char *t_error_str = error_repr(err);
    size_t len = ft_strlen(reason) + ft_strlen(t_error_str) +
                 ft_strlen("minishell: : \n");
    t_string *msg = string_new_with_capacity(len);

    if (!msg)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(reason, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putstr_fd(t_error_str, STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
    }

    string_extend(&msg, "minishell: ");
    string_extend(&msg, reason);
    string_extend(&msg, ": ");
    string_extend(&msg, t_error_str);
    string_extend(&msg, "\n");
    write(STDERR_FILENO, msg->data, msg->len);
}
