/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_is_directory.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:49:11 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:49:12 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <sys/stat.h>

bool	file_is_directory(const char *file_path)
{
	struct stat	file_stats;

	if (stat(file_path, &file_stats) < 0)
		return (false);
	return (S_ISDIR(file_stats.st_mode));
}
