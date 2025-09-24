NAME = libgnl.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -MMD -MP

# object directory
OBJ_DIR = obj

SRC = get_next_line.c get_next_line_utils.c

# object and dependency files
OBJ = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))
DEPS = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re