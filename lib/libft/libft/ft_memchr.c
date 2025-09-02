/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 09:04:01 by blohrer           #+#    #+#             */
/*   Updated: 2024/10/17 09:57:59 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int value, size_t num)
{
	const unsigned char	*p = (const unsigned char *)ptr;
	size_t				i;

	i = 0;
	while (i < num)
	{
		if (p[i] == (unsigned char)value)
		{
			return ((void *)(p + i));
		}
		i++;
	}
	return (NULL);
}
