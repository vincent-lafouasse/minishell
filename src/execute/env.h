#pragma once

#include "error/t_error.h"

typedef struct s_env_entry {
	const char	*key;
	const char	*value;
} t_env_entry;

typedef struct s_env {
	t_env_entry	entry;
	struct s_env	*next;
} t_env;

t_env				*from_envp(const char *values[]);

const t_env_entry	*env_get(t_env **env, const char *key);
t_env_entry			*env_get_mut(t_env **env, const char *key);
t_error				env_insert(t_env **env, const char *key, const char *value);
t_error				env_insert_envp_value(t_env **env, const char *str);
t_env_entry			*env_pop(t_env **env, const char *key);

t_error				env_make_envp(t_env **env, char *out[]);

void				env_clear(t_env **env);
