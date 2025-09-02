/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_pointer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 07:35:24 by blohrer           #+#    #+#             */
/*   Updated: 2024/11/18 10:19:08 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_put_pointer(void *ptr, int *counter)
{
	unsigned long	address;

	address = (unsigned long) ptr;
	ft_putstr("0x", counter);
	if (*counter == -1)
		return ;
	if (address == 0)
	{
		ft_putchar('0', counter);
		return ;
	}
	ft_puthex(address, 0, counter);
}
