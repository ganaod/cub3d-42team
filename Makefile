NAME        = cub3D
TEST_NAME   = test
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

LIBFT_PATH  = ./lib/libft/libft
PRINTF_PATH = ./lib/libft/ft_printf
GNL_PATH    = ./lib/libft/gnl
MLX_PATH    = ./lib/MLX42

INCLUDES    = -I ./include -I $(MLX_PATH)/include -I $(LIBFT_PATH)/include

LIBFT_A     = $(LIBFT_PATH)/libft.a
PRINTF_A    = $(PRINTF_PATH)/libftprintf.a
GNL_A       = $(GNL_PATH)/libgnl.a
MLX_A       = $(MLX_PATH)/build/libmlx42.a

LIBRARIES   = $(MLX_A) \
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
				render.c \
				render_column.c \
				render_dda.c \
				render_dda_setup.c \
				render_pixel_buffer.c \
				render_projection.c \
				render_ray_cast.c \
				player.c \
				player_utils.c \
				render_texture.c

SRCS        = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS        = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A) $(PRINTF_A) $(GNL_A) $(MLX_A)
	@$(CC) -g -fsanitize=address $(OBJS) $(LIBRARIES) -o $(NAME) > /dev/null
	@echo "\033[1;34m Built $(NAME) successfully!\033[0m"

# --- LIBRARY RULES WITH PROGRESS BAR ---
libs: $(LIBFT_A) $(PRINTF_A) $(GNL_A) $(MLX_A)

$(LIBFT_A):
	@printf "\033[1m\033[38;5;117mLoading libs: "
	@{ \
	  set -e; \
	  trap 'kill $$bar 2>/dev/null; wait $$bar 2>/dev/null || true' EXIT; \
	  while :; do printf "█"; sleep 0.15; done & bar=$$!; \
	  $(MAKE) -C $(LIBFT_PATH) > /dev/null; \
	}; printf "\033[0m\n\033[1;36mLibft ready!\033[0m\n"

$(PRINTF_A):
	@$(MAKE) -C $(PRINTF_PATH) > /dev/null
	@echo "\033[1;36mFt_printf ready!\033[0m"

$(GNL_A):
	@$(MAKE) -C $(GNL_PATH) > /dev/null
	@echo "\033[1;36mGNL ready!\033[0m"

$(MLX_A):
	@cmake -S $(MLX_PATH) -B $(MLX_PATH)/build -Wno-dev > /dev/null
	@$(MAKE) -C $(MLX_PATH)/build -j4 > /dev/null
	@echo "\033[1;36mMLX42 ready!\033[0m"

# --- OBJECT RULE ---
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) > /dev/null
	@echo "\033[1;32m✔ Compiled: $<\033[0m"

# --- CLEAN RULES ---
clean:
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_PATH) clean > /dev/null
	@$(MAKE) -C $(PRINTF_PATH) clean > /dev/null
	@$(MAKE) -C $(GNL_PATH) clean > /dev/null

fclean: clean
	@rm -f $(NAME) $(TEST_NAME)
	@$(MAKE) -C $(LIBFT_PATH) fclean > /dev/null
	@$(MAKE) -C $(PRINTF_PATH) fclean > /dev/null
	@$(MAKE) -C $(GNL_PATH) fclean > /dev/null
	@rm -rf $(MLX_PATH)/build
	@echo "\033[1;33m Fully cleaned $(NAME)!\033[0m"

re: fclean all

.PHONY: all clean fclean re libs
