/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 07:36:34 by blohrer           #+#    #+#             */
/*   Updated: 2024/11/26 09:04:54 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_handle_format(char format, va_list args, int *len)
{
	if (format == 'c')
		ft_putchar(va_arg(args, int), len);
	else if (format == 's')
		ft_putstr(va_arg(args, char *), len);
	else if (format == 'd' || format == 'i')
		ft_putnbr(va_arg(args, int), len);
	else if (format == 'u')
		ft_put_unsigned(va_arg(args, unsigned int), len);
	else if (format == 'x')
		ft_puthex(va_arg(args, unsigned int), 0, len);
	else if (format == 'X')
		ft_puthex(va_arg(args, unsigned int), 1, len);
	else if (format == 'p')
		ft_put_pointer(va_arg(args, void *), len);
	else if (format == '%')
		ft_putchar('%', len);
}

int	ft_printf(const char *format, ...)
{
	int		len;
	va_list	args;

	len = 0;
	va_start(args, format);
	while (*format && len != -1)
	{
		if (*format == '%' && *(format + 1))
		{
			format++;
			ft_handle_format(*format, args, &len);
		}
		else
		{
			ft_putchar(*format, &len);
		}
		format++;
	}
	va_end(args);
	if (len == -1)
		return (-1);
	return (len);
}

// int main(void)
// {
//     int ret_ft_printf;
//     int ret_printf;

//     ret_ft_printf = ft_printf("ft_printf - Char: %c\n", 'A');
//     ret_printf = printf("printf    - Char: %c\n", 'A');
//     printf("Return Werte: ft_printf = %d, printf = %d\n\n", ret_ft_printf,
// ret_printf);

//     ret_ft_printf = ft_printf("ft_printf - String: %s\n", "Hello, world!");
//     ret_printf = printf("printf    - String: %s\n", "Hello, world!");
//     printf("Return Werte: ft_printf = %d, printf = %d\n\n", ret_ft_printf,
// ret_printf);

//     ret_ft_printf = ft_printf("ft_printf - NULL-String: %s\n", NULL);
//     ret_printf = printf("printf    - NULL-String: %s\n", NULL);
//     printf("Return Werte: ft_printf = %d, printf = %d\n\n", ret_ft_printf,
// ret_printf);

//     ret_ft_printf = ft_printf("ft_printf - Decimal: %d, Integer: %i\n",
// 12345, -6789);
//     ret_printf = printf("printf    - Decimal: %d, Integer: %i\n", 12345,
// -6789);
//     printf("Return Werte: ft_printf = %d, printf = %d\n\n", ret_ft_printf,
// ret_printf);

//     ret_ft_printf = ft_printf("ft_printf - Unsigned: %u\n", 4294967295U);
//     ret_printf = printf("printf    - Unsigned: %u\n", 4294967295U);
//     printf("Return Werte: ft_printf = %d, printf = %d\n\n", ret_ft_printf,
// ret_printf);

//     ret_ft_printf = ft_printf("ft_printf - Hex Lowercase: %x, Uppercase:
// %X\n", 24433U, -10);
//     ret_printf = printf("printf    - Hex Lowercase: %x, Uppercase: %X\n",
// 24433U, -10);
//     printf("Return Werte: ft_printf = %d, printf = %d\n\n", ret_ft_printf,
// ret_printf);

//     int x = 42;
//     ret_ft_printf = ft_printf("ft_printf - Pointer: %p\n", &x);
//     ret_printf = printf("printf    - Pointer: %p\n", &x);
//     printf("Return Werte: ft_printf = %d, printf = %d\n\n", ret_ft_printf,
// ret_printf);

//     ret_ft_printf = ft_printf("ft_printf - Percent: %%\n");
//     ret_printf = printf("printf    - Percent: %%\n");
//     printf("Return Werte: ft_printf = %d, printf = %d\n\n", ret_ft_printf,
// ret_printf);

//     return (0);
// }
