/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_internals.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:51:57 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 16:55:00 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_INTERNALS_H
# define EXPORT_INTERNALS_H

# include <stddef.h>

void	report_invalid_identifier(char *identifier_name);

// returns 0 if assignment is malformated in any way
size_t	name_len(char *assignment);

#endif
