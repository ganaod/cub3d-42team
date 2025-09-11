/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_grid_build.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 09:17:36 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/11 11:06:15 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	compute_dims_from_lines(char **lines, int *out_w, int *out_h)
{
	int	i;
	int	w;

	if (!lines || !out_w || !out_h)
		return (0);
	i = 0;
	while (lines[i])
		i++;
	if (i <= 0)
		return (0);
	w = 0;
	while (lines[0][w] != '\0')
		w++;
	*out_w = w;
	*out_h = i;
	return (1);
}

static int	alloc_grid(t_map *m, int w, int h)
{
	int	total;

	if (!m || w <= 0 || h <= 0)
		return (0);
	total = w * h;
	m->grid = (int *)malloc(sizeof(int) * total);
	if (!m->grid)
		return (0);
	m->width = w;
	m->height = h;
	return (1);
}

static void	free_grid(t_map *m)
{
	if (m && m->grid)
	{
		free(m->grid);
		m->grid = NULL;
	}
}

int	build_grid_from_lines(t_map *m, t_player *pl, char **lines,
		int *player_found)
{
	int	w;
	int	h;
	int	count;

	if (!m || !pl || !lines || !player_found)
		return (0);
	if (!compute_dims_from_lines(lines, &w, &h))
		return (parse_error("Map error: empty map"), 0);
	if (!alloc_grid(m, w, h))
		return (parse_error("Map error: grid allocation failed"), 0);
	if (!fill_grid(m, pl, lines, &count))
	{
		free_grid(m);
		return (parse_error("Map error: inval character or multiple players"),
			0);
	}
	*player_found = count;
	if (count != 1)
	{
		free_grid(m);
		return (parse_error("Player error: exactly one start (N/S/E/W)"),
			0);
	}
	return (1);
}
