/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 08:54:38 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/11 08:54:48 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	free_lines_array(char **lines, int h)
{
	int i;

	i = 0;
	while (lines && i < h)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

void	free_map(t_map *m)
{
	int i;

	if (!m)
		return ;
	i = 0;
	while (i < 4)
	{
		free(m->texture_paths[i]);
		m->texture_paths[i] = NULL;
		i++;
	}
	if (m->grid)
	{
		free(m->grid);
		m->grid = NULL;
	}
	m->width = 0;
	m->height = 0;
}
