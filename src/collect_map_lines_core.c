/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_map_lines_core.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:29:28 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/11 11:09:40 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	free_lines_array(char **lines, int h)
{
	int	i;

	if (!lines)
		return ;
	i = 0;
	while (i < h)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
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

static int	read_lines_loop(int fd, char ***lines, int *cap, int *h)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		rstrip_eol(line);
		if (line[0] == '\0' || !validate_map_line(line))
		{
			free(line);
			return (0);
		}
		if (!append_line(lines, cap, h, line))
		{
			free(line);
			return (0);
		}
		line = get_next_line(fd);
	}
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
