/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:45:34 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/22 09:41:11 by blohrer          ###   ########.fr       */
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
	int	j;

	if (!s || !i || !out)
		return (0);
	j = *i;
	if (s[j] == '+')
		j++;
	if (!ft_isdigit((unsigned char)s[j]))
		return (0);
	val = 0;
	while (ft_isdigit((unsigned char)s[j]))
	{
		val = val * 10 + (s[j] - '0');
		if (val > 255)
			return (0);
		j++;
	}
	*i = j;
	*out = val;
	return (1);
}

int	expect_comma(const char *s, int *i)
{
	*i = skip_ws(s, *i);
	if (s[*i] != ',')
		return (0);
	(*i)++;
	*i = skip_ws(s, *i);
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
	*out_rgb = (0xFF << 24) | (b << 16) | (g << 8) | r;
	return (1);
}

/* opacity (%) | hex (alpha)
100 | 0xFF
50 | 0x80
25 | 0x40 */
