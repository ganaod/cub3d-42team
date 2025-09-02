/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:31:10 by blohrer           #+#    #+#             */
/*   Updated: 2024/11/18 11:32:27 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_puthex(unsigned long n, int uppercase, int *len)
{
	if (*len == -1)
		return ;
	if (n >= 16)
	{
		ft_puthex(n / 16, uppercase, len);
		if (*len == -1)
			return ;
	}
	if (uppercase)
		ft_putchar("0123456789ABCDEF"[n % 16], len);
	else
		ft_putchar("0123456789abcdef"[n % 16], len);
}
