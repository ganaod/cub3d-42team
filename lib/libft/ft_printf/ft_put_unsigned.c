/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_unsigned.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 07:34:23 by blohrer           #+#    #+#             */
/*   Updated: 2024/11/18 10:14:51 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_put_unsigned(unsigned int n, int *counter)
{
	if (*counter == -1)
		return ;
	if (n >= 10)
	{
		ft_put_unsigned(n / 10, counter);
		if (*counter == -1)
			return ;
	}
	ft_putchar((n % 10) + '0', counter);
}
