# project metadata
NAME = cub3d

# directory structure
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

# library paths
LIBFT_DIR = lib/libft
PRINTF_DIR = lib/ft_printf  
GNL_DIR = lib/gnl
MLX_DIR = lib/MLX42
MLX_BUILD = $(MLX_DIR)/build

# explicit source file list (42 norm requirement)
SRC_FILES = main.c \
			global_state.c \
			parse_utils.c \
			parse_header.c \
			parse_header_lines.c \
			parse_header_utils.c \
			parse_paths_check.c \
			collect_map_lines.c \
			collect_map_lines_utils.c \
			collect_map_lines_core.c \
			normalize_map.c \
			map_grid_cells.c \
			map_grid_fill.c \
			map_grid_build.c \
			map_check.c \
			map_check_flood.c \
			map_check_utils.c \
			map_check_border.c \
			graphics_window.c \
			utils.c \
			player.c \
			player_utils.c \
			render.c \
			render_column.c \
			render_dda.c \
			render_dda_setup.c \
			render_pixel_buffer.c \
			render_projection.c \
			render_ray_cast.c \
			render_texture.c \
			render_texture_utils.c \
			load_texture.c \
			cleanup.c \
			map_loader.c \
			runtime_init.c

# derived file paths
SRCS = $(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJS = $(addprefix $(OBJ_DIR)/,$(SRC_FILES:.c=.o))
DEPS = $(OBJS:.o=.d)

# library targets
LIBFT = $(LIBFT_DIR)/libft.a
PRINTF = $(PRINTF_DIR)/libftprintf.a
GNL = $(GNL_DIR)/libgnl.a
MLX = $(MLX_BUILD)/libmlx42.a

# toolchain configuration
CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP
INCLUDES = -I $(INC_DIR) -I $(MLX_DIR)/include -I $(LIBFT_DIR)
LDFLAGS = -L$(LIBFT_DIR) -lft \
		  -L$(PRINTF_DIR) -lftprintf \
		  -L$(GNL_DIR) -lgnl \
		  -L$(MLX_BUILD) -lmlx42 -lglfw -ldl -pthread -lm

# build rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(PRINTF) $(GNL) $(MLX)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# dependency inclusion
-include $(DEPS)

# library builds
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(PRINTF):
	$(MAKE) -C $(PRINTF_DIR)

$(GNL):
	$(MAKE) -C $(GNL_DIR)

$(MLX):
	mkdir -p $(MLX_BUILD)
	cmake -S $(MLX_DIR) -B $(MLX_BUILD)
	$(MAKE) -C $(MLX_BUILD)

# cleanup
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(PRINTF_DIR) clean
	$(MAKE) -C $(GNL_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(PRINTF_DIR) fclean
	$(MAKE) -C $(GNL_DIR) fclean
	rm -rf $(MLX_BUILD)

re: fclean all

.PHONY: all clean fclean re