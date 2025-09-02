# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kzarins <kzarins@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/29 10:58:28 by blohrer           #+#    #+#              #
#    Updated: 2025/04/08 15:37:09 by kzarins          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 	= libftprintf.a
CC 		= gcc
CFLAGS 	= -Wall -Wextra -Werror
AR = ar

CFILES =	ft_putchar.c \
			ft_putstr.c \
			ft_putnbr_fd.c \
			ft_puthex.c \
			ft_put_unsigned.c \
			ft_put_pointer.c \
			ft_printf.c \

OFILES = $(CFILES:.c=.o)

all: $(NAME)

$(NAME): $(OFILES)
	@$(AR) rcs $(NAME) $(OFILES) > /dev/null

%.o: %.c
	@$(CC) -c $(CFLAGS) $? > /dev/null

clean:
	@rm -f $(OFILES) > /dev/null

fclean: clean
	@rm -f $(NAME) $(OBFILES) > /dev/null

re: fclean all

.PHONY: all clean fclean re
