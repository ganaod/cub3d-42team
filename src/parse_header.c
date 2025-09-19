/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:31:12 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/19 10:19:50 by go-donne         ###   ########.fr       */
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
	parse_error("Header error: unknown identifier (expected NO/SO/WE/EA/F/C)");
	return (0);
}

int	match2(const char *s, int i, char a, char b)
{
	if (!s || !s[i] || !s[i + 1])
		return (0);
	return (s[i] == a && s[i + 1] == b);
}

static int	apply_tex(char **slot, const char *after_key, int bit, int *flags)
{
	if (*flags & bit)
		return (parse_error("Header error: duplicate entry"), 0);
	if (!set_texture_field(slot, after_key))
		return (parse_error("Texture error: path missing or invalid"), 0);
	*flags |= bit;
	return (1);
}

int	parse_header_texture(t_map *m, const char *line, int i, int *flags)
{
	if (match2(line, i, 'N', 'O'))
		return (apply_tex(&m->texture_paths[NORTH], line + i + 2, HDR_NO,
				flags));
	if (match2(line, i, 'S', 'O'))
		return (apply_tex(&m->texture_paths[SOUTH], line + i + 2, HDR_SO,
				flags));
	if (match2(line, i, 'W', 'E'))
		return (apply_tex(&m->texture_paths[WEST], line + i + 2, HDR_WE,
				flags));
	if (match2(line, i, 'E', 'A'))
		return (apply_tex(&m->texture_paths[EAST], line + i + 2, HDR_EA,
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
			return (parse_error("Header error: duplicate F entry"), 0);
		if (!set_color_field(&m->floor_color, line + i + 1, &f_set))
			return (parse_error("Color error: F must be R,G,B with 0-255"), 0);

		// DEBUG: Verify parsing happened
		printf("DEBUG: FLOOR COLOUR PARSED = 0x%08X\n", m->floor_color);
			
		*flags |= HDR_F;
		return (1);
	}
	if (line[i] == 'C')
	{
		if (*flags & HDR_C)
			return (parse_error("Header error: duplicate C entry"), 0);
		if (!set_color_field(&m->ceiling_color, line + i + 1, &f_set))
			return (parse_error("Color error: C must be R,G,B with 0-255"), 0);

		// DEBUG: Verify parsing happened
		printf("DEBUG: CEILING COLOUR PARSED = 0x%08X\n", m->ceiling_color);
			
		*flags |= HDR_C;
		return (1);
	}
	return (0);
}
