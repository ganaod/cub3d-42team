/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 08:21:52 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/19 15:28:47 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	set_texture_field(char **dst_path, const char *after_key)
{
	int		i;
	int		start;
	int		end;
	char	*path;

	if (!after_key || *dst_path)
		return (0);
	i = skip_ws(after_key, 0);
	start = i;
	end = -1;
	while (after_key[i])
	{
		if (after_key[i] != ' ' && after_key[i] != '\t' && after_key[i] != '\r'
			&& after_key[i] != '\n')
			end = i;
		i++;
	}
	if (end < start)
		return (0);
	path = ft_substr(after_key, start, end - start + 1);
	if (!path)
		return (0);
	*dst_path = path;
	return (1);
}

static int	dup_trimmed_color_token(const char *after_key, char **out)
{
	int	i;
	int	j;
	int	end;

	if (!after_key || !out)
		return (0);
	i = skip_ws(after_key, 0);
	j = i;
	end = -1;
	while (after_key[j])
	{
		if (after_key[j] != ' ' && after_key[j] != '\t'
			&& after_key[j] != '\r' && after_key[j] != '\n')
			end = j;
		j++;
	}
	if (end < i)
		return (0);
	*out = ft_substr(after_key, i, end - i + 1);
	return (*out != NULL);
}

int	set_color_field(uint32_t *dst_rgb, const char *after_key, int *was_set)
{
	char		*tmp;
	uint32_t	c;

	if (!dst_rgb || !after_key || !was_set || *was_set)
		return (0);
	if (!dup_trimmed_color_token(after_key, &tmp))
		return (0);
	if (!parse_rgb_triplet(tmp, &c))
	{
		free(tmp);
		return (0);
	}
	*dst_rgb = c;
	*was_set = 1;
	free(tmp);
	return (1);
}
