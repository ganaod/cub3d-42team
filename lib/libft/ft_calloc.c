/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 09:33:11 by blohrer           #+#    #+#             */
/*   Updated: 2024/10/17 09:49:30 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	size_t			total_size;
	void			*ptr;
	unsigned char	*p;
	size_t			i;

	total_size = num * size;
	ptr = malloc(total_size);
	if (ptr == NULL)
	{
		return (NULL);
	}
	p = (unsigned char *)ptr;
	i = 0;
	while (i < total_size)
	{
		p[i] = 0;
		i++;
	}
	return (ptr);
}
