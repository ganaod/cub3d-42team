/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_paths_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:36:53 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/15 10:45:13 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	check_one_texture(const t_map *m, int i)
{
	int fd;

	if (!m->texture_paths[i] || m->texture_paths[i][0] == '\0')
	{
		parse_error("texture path missing");
		return (0);
	}
	fd = open(m->texture_paths[i], O_RDONLY);
	if (fd < 0)
	{
		if (i == 0)
			parse_error("texture NO not found");
		else if (i == 1)
			parse_error("texture SO not found");
		else if (i == 2)
			parse_error("texture WE not found");
		else
			parse_error("texture EA not found");
		return (0);
	}
	close(fd);
	return (1);
}

int	check_texture_paths_exist(const t_map *m)
{
	int i;

	if (!m)
		return (0);
	i = 0;
	while (i < 4)
	{
		if (!check_one_texture(m, i))
			return (0);
		i++;
	}
	return (1);
}
