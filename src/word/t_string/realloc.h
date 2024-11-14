/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamar <jamar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 20:01:32 by jamar             #+#    #+#             */
/*   Updated: 2024/03/11 23:18:35 by jamar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REALLOC_H
# define REALLOC_H

# include <stddef.h>

void	*ft_realloc(void *old, size_t len, size_t new_size);

#endif // REALLOC_H
