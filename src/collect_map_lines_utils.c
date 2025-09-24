/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_map_lines_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:43:47 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/24 12:19:16 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	is_valid_map_char(char c)
{
	if (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W'
		|| c == ' ')
		return (1);
	return (0);
}

int	validate_map_line(const char *s)
{
	int	i;
	int	has_any;

	if (!s)
		return (0);
	if (s[0] == '\0')
		return (0);
	i = 0;
	has_any = 0;
	while (s[i] != '\0')
	{
		if (!is_valid_map_char(s[i]))
			return (0);
		has_any = 1;
		i++;
	}
	if (has_any)
		return (1);
	return (0);
}

void	rstrip_eol(char *s)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i] != '\0')
		i++;
	if (i == 0)
		return ;
	i--;
	while (i >= 0 && (s[i] == '\n' || s[i] == '\r'))
	{
		s[i] = '\0';
		i--;
	}
}
