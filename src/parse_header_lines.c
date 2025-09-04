/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header_lines.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:00:53 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/04 08:13:18 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	is_map_line(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = skip_ws(s, 0);
	if (s[i] == '\0')
		return (0);
	if (s[i] == '0' || s[i] == '1')
		return (1);
	return (0);
}

static int	header_complete(int flags)
{
	int	need;

	need = HDR_NO | HDR_SO | HDR_WE | HDR_EA | HDR_F | HDR_C;
	return ((flags & need) == need);
}

static int	consume_header_line(t_map *m, char *line, int *flags)
{
	int	i;

	i = skip_ws(line, 0);
	if (line[i] == '\0' || line[i] == '\n' || line[i] == '\r' || line[i] == '#')
		return (1);
	return (parse_header_line(m, line, flags));
}

static int	read_header_loop(t_map *m, int fd, int *flags)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		if (is_map_line(line))
		{
			m->first_map_line = line;
			return (1);
		}
		if (!consume_header_line(m, line, flags))
		{
			free(line);
			return (0);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (1);
}

int	parse_header_lines(t_map *m, int fd)
{
	int	flags;

	if (!m || fd < 0)
		return (0);
	m->first_map_line = NULL;
	flags = 0;
	if (!read_header_loop(m, fd, &flags))
		return (0);
	if (!header_complete(flags))
		return (0);
	return (1);
}
