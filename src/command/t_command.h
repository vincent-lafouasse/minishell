#ifndef COMMAND_H
# define COMMAND_H

#include <stddef.h>

typedef enum e_command_type t_command_type;
typedef enum e_operator_kind t_operator_kind;
typedef struct s_logic_node t_logic_node;
typedef struct s_simple_command t_simple_command;
typedef union u_any_command t_any_command;
typedef struct s_command t_command;

enum e_command_type {
    SIMPLE,
    SUBSHELL,
    LOGIC,
    PIPE,
    SEPARATOR
};

typedef enum e_logic_node_kind {
    AND,
    OR
} t_logic_node_kind;

typedef struct s_logic_node {
    t_logic_node_kind    kind;
    t_command            *left;
    t_command            *right;
} t_logic_node;

// NOTE: `words` must be null terminated!
typedef struct s_simple_command {
    int /* ?? */    *redirections;
    char            **words;
} t_simple_command;

typedef union u_any_command {
    struct s_simple_command    simple;
    struct s_logic_node    simple__;
} t_any_command;

typedef struct s_command {
    enum e_command_type    type;
    struct s_command *left;
    struct s_command *right;
    union u_any_command    command;
} t_command;

#endif // COMMAND_H
