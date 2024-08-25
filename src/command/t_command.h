#ifndef COMMAND_H
#define COMMAND_H

#include <stddef.h>

typedef enum e_command_type t_command_type;
typedef enum e_logic_node_kind t_logic_node_kind;
typedef enum e_operator_kind t_operator_kind;
typedef struct s_logic_node t_logic_node;
typedef struct s_simple_command t_simple_command;
typedef union u_any_command t_any_command;
typedef struct s_command t_command;

enum e_command_type
{
    SIMPLE,
    SUBSHELL,
    LOGIC,
    PIPE,
    SEPARATOR
};

enum e_logic_node_kind
{
    AND,
    OR
};

struct s_logic_node
{
    t_logic_node_kind kind;
    t_command *left;
    t_command *right;
};

// NOTE: `words` must be null terminated!
struct s_simple_command
{
    int /* ?? */ *redirections;
    char **words;
};

union u_any_command
{
    t_simple_command simple;
    t_logic_node logic;
};

struct s_command
{
    t_command_type type;
    t_command *left;
    t_command *right;
    t_any_command command;
};

#endif // COMMAND_H
