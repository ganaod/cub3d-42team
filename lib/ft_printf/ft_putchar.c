/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:54:22 by blohrer           #+#    #+#             */
/*   Updated: 2024/11/18 11:28:23 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putchar(char c, int *len)
{
	int	result;

	result = write(1, &c, 1);
	if (result == -1)
	{
		*len = -1;
	}
	else
	{
		*len += result;
	}
}
