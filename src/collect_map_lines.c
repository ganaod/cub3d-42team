/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_map_lines.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:03:55 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/05 08:45:59 by blohrer          ###   ########.fr       */
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
	int		new_cap;
	int		i;

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
