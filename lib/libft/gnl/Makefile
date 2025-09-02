# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kzarins <kzarins@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/29 10:09:34 by blohrer           #+#    #+#              #
#    Updated: 2025/04/08 15:36:54 by kzarins          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libgnl.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -I.

SRC = get_next_line.c get_next_line_utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ) > /dev/null

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ > /dev/null

clean:
	@rm -f $(OBJ) > /dev/null

fclean: clean
	@rm -f $(NAME) > /dev/null

re: fclean all

.PHONY: all clean fclean re
