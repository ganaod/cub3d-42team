/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 14:24:47 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/24 16:03:25 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	map_open_and_parse_header(t_game *g, const char *path, int *fd_out)
{
	int	fd;

	if (!g || !path || !fd_out)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		parse_error("open failed");
		return (0);
	}
	if (!parse_header_lines(&g->map, fd))
	{
		parse_error("header parse failed");
		close(fd);
		return (0);
	}
	*fd_out = fd;
	return (1);
}

static int	map_collect_lines_close(t_game *g, int fd, char ***out_lines,
		int *out_h)
{
	if (!g || !out_lines || !out_h)
		return (0);
	*out_lines = NULL;
	*out_h = 0;
	if (!collect_map_lines(&g->map, fd, out_lines, out_h))
	{
		parse_error("collect_map_lines failed");
		close(fd);
		free_map(g, &g->map);
		return (0);
	}
	close(fd);
	return (1);
}

static int	map_normalize_lines_only(t_game *g, char ***lines_io, int h)
{
	if (!g || !lines_io || !*lines_io)
		return (0);
	if (!normalize_map(lines_io, h, &g->map.width))
	{
		parse_error("normalize_map failed");
		free_lines_array(*lines_io, h);
		free_map(g, &g->map);
		return (0);
	}
	return (1);
}

static int	map_build_grid_and_check(t_game *g, char **lines, int h)
{
	int	player_found;

	player_found = 0;
	if (!build_grid_from_lines(&g->map, &g->player, lines, &player_found)
		|| player_found != 1)
	{
		parse_error("grid build failed (player count)");
		free_lines_array(lines, h);
		free_map(g, &g->map);
		return (0);
	}
	if (!map_is_closed(&g->map))
	{
		parse_error("map is open (leaks into VOID)");
		free_lines_array(lines, h);
		free_map(g, &g->map);
		return (0);
	}
	free_lines_array(lines, h);
	return (1);
}

int	map_load_and_validate(t_game *g, const char *path)
{
	int		fd;
	char	**lines;
	int		h;

	if (!map_open_and_parse_header(g, path, &fd))
		return (0);
	if (!map_collect_lines_close(g, fd, &lines, &h))
		return (0);
	if (!map_normalize_lines_only(g, &lines, h))
		return (0);
	if (!map_build_grid_and_check(g, lines, h))
		return (0);
	return (1);
}
