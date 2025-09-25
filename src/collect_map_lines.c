/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_map_lines.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:03:55 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 20:15:45 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	ensure_array_initialized(char ***lines_ptr, int *cap_ptr,
		int *h_ptr)
{
	if (!lines_ptr || !cap_ptr || !h_ptr)
		return (0);
	if (*lines_ptr != NULL)
		return (1);
	*cap_ptr = 16;
	*lines_ptr = (char **)malloc(sizeof(char *) * (*cap_ptr));
	if (!*lines_ptr)
		return (0);
	(*lines_ptr)[0] = NULL;
	*h_ptr = 0;
	return (1);
}

static int	grow_if_needed(char ***lines_ptr, int *cap_ptr, int h)
{
	char	**new_lines;
	int		i;
	int		new_cap;

	if (!lines_ptr || !*lines_ptr || !cap_ptr)
		return (0);
	if (h + 1 < *cap_ptr)
		return (1);
	new_cap = (*cap_ptr) * 2;
	new_lines = (char **)malloc(sizeof(char *) * new_cap);
	if (!new_lines)
		return (0);
	i = 0;
	while (i < h)
	{
		new_lines[i] = (*lines_ptr)[i];
		i++;
	}
	new_lines[h] = NULL;
	free(*lines_ptr);
	*lines_ptr = new_lines;
	*cap_ptr = new_cap;
	return (1);
}

static int	adopt_first_map_line(t_map *m, char ***lines, int *cap, int *h)
{
	if (!m || !lines || !cap || !h || !m->first_map_line)
		return (0);
	rstrip_eol(m->first_map_line);
	if (!validate_map_line(m->first_map_line))
		return (0);
	if (!append_line(lines, cap, h, m->first_map_line))
		return (0);
	m->first_map_line = NULL;
	return (1);
}

int	collect_map_lines(t_map *m, int fd, char ***out_lines, int *out_h)
{
	char	**lines;
	int		cap;
	int		h;

	if (!m || fd < 0 || !out_lines || !out_h)
		return (0);
	lines = NULL;
	cap = 0;
	h = 0;
	if (!adopt_first_map_line(m, &lines, &cap, &h))
	{
		if (!m->first_map_line)
			return (parse_error("Map error: missing map after header"), 0);
		return (parse_error("Map error: invalid first map line"), 0);
	}
	if (!read_lines_loop(fd, &lines, &cap, &h))
	{
		free_lines_array(lines, h);
		return (parse_error("Map error: invalid or empty line inside map"), 0);
	}
	*out_lines = lines;
	*out_h = h;
	return (1);
}

int	append_line(char ***lines_ptr, int *cap_ptr, int *h_ptr, char *take)
{
	if (!take)
		return (0);
	if (!ensure_array_initialized(lines_ptr, cap_ptr, h_ptr))
		return (0);
	if (!grow_if_needed(lines_ptr, cap_ptr, *h_ptr))
		return (0);
	(*lines_ptr)[*h_ptr] = take;
	*h_ptr = *h_ptr + 1;
	(*lines_ptr)[*h_ptr] = NULL;
	return (1);
}
