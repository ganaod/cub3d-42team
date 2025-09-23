/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 08:28:47 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/23 14:22:44 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	idx_2d_to_1d(int x, int y, int w)
{
	return (y * w + x);
}

int	in_bounds(int x, int y, int w, int h)
{
	return (x >= 0 && y >= 0 && x < w && y < h);
}

void	queue_push(int *q, int *tail, int v)
{
	q[*tail] = v;
	*tail += 1;
}

int	queue_pop(int *q, int *head, int tail, int *out)
{
	if (*head == tail)
		return (0);
	*out = q[*head];
	*head += 1;
	return (1);
}

int	load_textures_or_fail(t_game *g)
{
	if (!g)
		return (0);
	if (!load_all_wall_textures(&g->map))
	{
		parse_error("failed to load wall textures");
		return (0);
	}
	return (1);
}
