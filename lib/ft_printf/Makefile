NAME = libftprintf.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -MMD -MP
AR = ar

# object directory
OBJ_DIR = obj

CFILES = ft_putchar.c \
	ft_putstr.c \
	ft_putnbr_fd.c \
	ft_puthex.c \
	ft_put_unsigned.c \
	ft_put_pointer.c \
	ft_printf.c

# object and dependency files
OBJ = $(addprefix $(OBJ_DIR)/,$(CFILES:.c=.o))
DEPS = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) rcs $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re