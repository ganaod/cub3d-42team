/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 20:32:02 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 20:32:44 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	mpr_prepare_buffers(const t_map *m, char **vis_void, int **q)
{
	int	total;
	int	i;

	total = m->width * m->height;
	*vis_void = (char *)malloc(total);
	*q = (int *)malloc(sizeof(int) * total);
	if (!*vis_void || !*q)
	{
		free(*vis_void);
		free(*q);
		return (0);
	}
	i = 0;
	while (i < total)
	{
		(*vis_void)[i] = 0;
		i++;
	}
	return (1);
}

int	map_is_closed_player_region(const t_map *m, const t_player *pl)
{
	char	*vis_void;
	int		*q;
	int		ok;

	if (!m || !pl || !m->grid || m->width <= 0 || m->height <= 0)
		return (0);
	vis_void = NULL;
	q = NULL;
	if (!mpr_prepare_buffers(m, &vis_void, &q))
		return (0);
	mark_outside_void_pure(m, vis_void, q);
	ok = player_region_is_closed(m, pl, vis_void);
	free(vis_void);
	free(q);
	return (ok);
}
