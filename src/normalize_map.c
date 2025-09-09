/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 10:09:54 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/09 11:08:27 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	compute_map_width(char **lines, int h, int *out_w)
{
	int	i;
	int	max_w;
	int	len;

	if (!lines || !out_w || h <= 0)
		return (0);
	i = 0;
	max_w = 0;
	while (i < h)
	{
		len = (int)ft_strlen(lines[i]);
		if (len > max_w)
			max_w = len;
		i++;
	}
	*out_w = max_w;
	return (max_w > 0);
}

static void	copy_and_pad_line(const char *src, char *dst, int w)
{
	int	j;

	j = 0;
	while (j < w && src[j] != '\0')
	{
		dst[j] = src[j];
		j++;
	}
	while (j < w)
	{
		dst[j] = ' ';
		j++;
	}
	dst[w] = '\0';
}

static void	free_rect_partial(char **arr, int filled)
{
	int	k;

	if (!arr)
		return ;
	k = 0;
	while (k < filled)
	{
		free(arr[k]);
		k++;
	}
	free(arr);
}

static int	build_rect_lines(char ***out_new, char **old_lines, int h, int w)
{
	char	**new_lines;
	int		i;

	if (!out_new || !old_lines || h <= 0 || w <= 0)
		return (0);
	new_lines = (char **)malloc(sizeof(char *) * (h + 1));
	if (!new_lines)
		return (0);
	i = 0;
	while (i < h)
	{
		new_lines[i] = (char *)malloc(sizeof(char) * (w + 1));
		if (!new_lines[i])
		{
			free_rect_partial(new_lines, i);
			return (0);
		}
		copy_and_pad_line(old_lines[i], new_lines[i], w);
		i++;
	}
	new_lines[h] = NULL;
	*out_new = new_lines;
	return (1);
}

int	normalize_map(char ***lines_io, int h, int *out_w)
{
	char	**old;
	char	**rect;
	int		w;
	int		i;

	if (!lines_io || !*lines_io || !out_w || h <= 0)
		return (0);
	old = *lines_io;
	if (!compute_map_width(old, h, &w))
		return (0);
	if (!build_rect_lines(&rect, old, h, w))
		return (0);
	i = 0;
	while (i < h)
	{
		free(old[i]);
		i++;
	}
	free(old);
	*lines_io = rect;
	*out_w = w;
	return (1);
}
