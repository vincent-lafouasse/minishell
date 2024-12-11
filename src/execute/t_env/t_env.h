#pragma once

#include "error/t_error.h"

typedef struct s_env_entry {
	const char	*key;
	const char	*value;
} t_env_entry;

typedef struct s_env {
	t_env_entry	entry;
	struct s_env	*next;
	struct s_env	*prev;
} t_env;

t_error				from_envp(const char *values[], t_env **out);

const t_env_entry	*env_get(const t_env *env, const char *key);
t_env_entry			*env_get_mut(t_env *env, const char *key);
t_error				env_insert(t_env **env, const char *key, const char *value);
t_error				env_insert_owned_kv(t_env **env, char *key, char *value);
bool				env_key_exists(const t_env *env, const char *key);
t_env				*env_remove(t_env **env, const char *key);

char				**env_make_path_or_empty(const t_env *env);
char				**env_make_envp(const t_env *env);

void				env_destroy(t_env **env);
