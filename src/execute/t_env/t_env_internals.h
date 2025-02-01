/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_env_internals.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:27:28 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:28:01 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_ENV_INTERNALS_H
# define T_ENV_INTERNALS_H

# include "t_env.h"

typedef void	(*t_destructor)(void *);

t_env	*env_new(t_env_entry entry);
t_env	*env_clone_link(const t_env *env);

t_error	env_push_front(t_env **env, t_env_entry entry);
void	env_push_front_link(t_env **env, t_env *link);
t_env	*env_find_node(t_env *env, const char *key);
t_env	*env_detach_link(t_env *env, t_env **head);
t_env	*env_pop_key(t_env **env, const char *key);

void	env_delone(t_env **env, t_destructor del);
void	env_clear(t_env **env, t_destructor del);

#endif // T_ENV_INTERNALS_H
