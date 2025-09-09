/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 08:28:47 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/09 08:29:28 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* Klein, inline-ähnlich: Index berechnen */
int	idx_2d_to_1d(int x, int y, int w)
{
	return (y * w + x);
}

/* Prüft, ob (x,y) innerhalb der Map liegt */
int	in_bounds(int x, int y, int w, int h)
{
	return (x >= 0 && y >= 0 && x < w && y < h);
}

/* Schiebt v in die Queue (tail inkrementieren, ringfrei,
	aber hier reicht linear) */
void	queue_push(int *q, int *tail, int v)
{
	q[*tail] = v;
	*tail += 1;
}

/* Holt ein Element aus der Queue; gibt 0 zurück, wenn leer,
	sonst 1 und setzt *out */
int	queue_pop(int *q, int *head, int tail, int *out)
{
	if (*head == tail)
		return (0);
	*out = q[*head];
	*head += 1;
	return (1);
}
