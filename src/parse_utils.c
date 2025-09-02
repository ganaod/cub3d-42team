/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:45:34 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/02 15:12:45 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	skip_ws(const char *s, int i)
{
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	return (i);
}

int	parse_u8_component(const char *s, int *i, int *out)
{
	int	val;
	int	start;

	start = *i;
	if (!ft_isdigit((unsigned char)s[*i]))
		return (0);
	val = 0;
	while (ft_isdigit((unsigned char)s[*i]))
	{
		val = val * 10 + (s[*i] - '0');
		if (val > 255)
			return (0);
		(*i)++;
	}
	if (*i == start)
		return (0);
	*out = val;
	return (1);
}

int expect_comma(const char *s, int *i)
{
	*i = skip_ws(s, *i);
	if (s[*i] != ',')
		return (0);
	(*i)++;
	return (1);
}

int	parse_rgb_triplet(const char *s, uint32_t *out_rgb)
{
	int	i;
	int	r;
	int	g;
	int	b;

	if (!s || !out_rgb)
		return (0);
	i = 0;
	i = skip_ws(s, i);
	if (!parse_u8_component(s, &i, &r))
		return (0);
	if (!expect_comma(s, &i))
		return (0);
	if (!parse_u8_component(s, &i, &g))
		return (0);
	if (!expect_comma(s, &i))
		return (0);
	if (!parse_u8_component(s, &i, &b))
		return (0);
	i = skip_ws(s, i);
	if (s[i] != '\0')
		return (0);
	*out_rgb = (r << 16) | (g << 8) | b;
	return (1);
}
