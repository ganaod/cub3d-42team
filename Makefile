NAME        = cub3D
TEST_NAME	= test
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

LIBFT_PATH  = ./lib/libft/libft
PRINTF_PATH = ./lib/libft/ft_printf
GNL_PATH    = ./lib/libft/gnl
MLX_PATH    = ./lib/MLX42


INCLUDES    = -I ./include -I $(MLX_PATH)/include -I $(LIBFT_PATH)/include

LIBRARIES   =  $(MLX_PATH)/build/libmlx42.a \
               -L$(LIBFT_PATH) -lft \
               -L$(PRINTF_PATH) -lftprintf \
               -L$(GNL_PATH) -lgnl \
               -L$(MLX_PATH)/build -lmlx42 -lglfw -ldl -pthread -lm

SRC_DIR     = src
SRC_FILES   =	main.c \
				global_state.c \
				parse_utils.c \
				parse_header.c \
				parse_header_lines.c \
				parse_header_utils.c \
				collect_map_lines.c \
				collect_map_lines_utils.c \
				collect_map_lines_core.c \
				normalize_map.c \
				map_grid_cells.c \
				map_grid_fill.c \
				map_grid_build.c \
				map_check_flood.c \
				map_check_utils.c \
				utils.c \
				map_check.c \
				parse_paths_check.c \
				render__main.c \
				render_column.c \
				render_dda.c \
				render_dda_setup.c \
				render_pixel_buffer.c \
				render_projection.c \
				render_ray_cast.c \
				render_texture.c

SRCS        = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS        = $(SRCS:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) > /dev/null
	@echo "\033[1;32m✔ Compiled: $<\033[0m"

$(NAME): libft ftprintf gnl mlx $(OBJS)
	@$(CC) -g -fsanitize=address $(OBJS) $(LIBRARIES) -o $(NAME) > /dev/null
	@echo "\033[1;34m Built $(NAME) successfully!\033[0m"

ftprintf:
	@make -C $(PRINTF_PATH)

libft:
	@make -C $(LIBFT_PATH)

gnl:
	@make -C $(GNL_PATH)

mlx:
	@cmake -S $(MLX_PATH) -B $(MLX_PATH)/build -Wno-dev > /dev/null
	@make -C $(MLX_PATH)/build -j4 > /dev/null
	@echo "\033[1;36m Built MLX42!\033[0m"


clean:
	@rm -f $(OBJS)
	@make -C $(LIBFT_PATH) clean > /dev/null
	@make -C $(PRINTF_PATH) clean > /dev/null
	@make -C $(GNL_PATH) clean > /dev/null

fclean: clean
	@rm -f $(NAME) $(TEST_NAME)
	@make -C $(LIBFT_PATH) fclean > /dev/null
	@make -C $(PRINTF_PATH) fclean > /dev/null
	@make -C $(GNL_PATH) fclean > /dev/null
	@rm -rf $(MLX_PATH)/build
	@echo "\033[1;33m Fully cleaned $(NAME)!\033[0m"

re: fclean all

all: $(NAME)

.PHONY: all clean fclean re libft gnl mlx ftprintf
