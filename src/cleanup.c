/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:41:44 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/23 13:49:20 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	free_lines_array(char **lines, int h)
{
	int	i;

	i = 0;
	while (i < h)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

void	free_map(t_map *m)
{
	int	i;

	if (!m)
		return ;
	free(m->grid);
	m->grid = NULL;
	i = 0;
	while (i < 4)
	{
		if (m->texture_paths[i])
			free(m->texture_paths[i]);
		m->texture_paths[i] = NULL;
		if (m->wall_textures[i].mlx_image && g_game.graphics.mlx)
		{
			mlx_delete_image(g_game.graphics.mlx,
				m->wall_textures[i].mlx_image);
			m->wall_textures[i].mlx_image = NULL;
		}
		if (m->wall_textures[i].pixels)
			free(m->wall_textures[i].pixels);
		m->wall_textures[i].pixels = NULL;
		m->wall_textures[i].image_width = 0;
		m->wall_textures[i].image_height = 0;
		i++;
	}
}

int	cleanup_and_exit(t_game *g, int code)
{
	if (g && g->graphics.mlx)
		mlx_terminate(g->graphics.mlx);
	if (g)
		free_map(&g->map);
	return (code);
}
