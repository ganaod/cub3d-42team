/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:31:12 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/04 08:23:38 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	parse_header_line(t_map *m, const char *line, int *flags)
{
	int	i;

	if (!m || !line || !flags)
		return (0);
	i = skip_ws(line, 0);
	if (!line[i])
		return (0);
	if (parse_header_texture(m, line, i, flags))
		return (1);
	if (parse_header_color(m, line, i, flags))
		return (1);
	return (0);
}

int	match2(const char *s, int i, char a, char b)
{
	if (!s || !s[i] || !s[i + 1])
		return (0);
	return (s[i] == a && s[i + 1] == b);
}

int	parse_header_texture(t_map *m, const char *line, int i, int *flags)
{
	if (match2(line, i, 'N', 'O'))
		return (set_tex_entry(&m->texture_paths[NORTH], line + i + 2, HDR_NO,
				flags));
	if (match2(line, i, 'S', 'O'))
		return (set_tex_entry(&m->texture_paths[SOUTH], line + i + 2, HDR_SO,
				flags));
	if (match2(line, i, 'W', 'E'))
		return (set_tex_entry(&m->texture_paths[WEST], line + i + 2, HDR_WE,
				flags));
	if (match2(line, i, 'E', 'A'))
		return (set_tex_entry(&m->texture_paths[EAST], line + i + 2, HDR_EA,
				flags));
	return (0);
}

int	parse_header_color(t_map *m, const char *line, int i, int *flags)
{
	int	f_set;

	f_set = 0;
	if (line[i] == 'F')
	{
		if (*flags & HDR_F)
			return (0);
		if (!set_color_field(&m->floor_color, line + i + 1, &f_set))
			return (0);
		*flags |= HDR_F;
		return (1);
	}
	if (line[i] == 'C')
	{
		if (*flags & HDR_C)
			return (0);
		if (!set_color_field(&m->ceiling_color, line + i + 1, &f_set))
			return (0);
		*flags |= HDR_C;
		return (1);
	}
	return (0);
}
