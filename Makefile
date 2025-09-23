# ==============================================================================
# Cub3D Makefile - Corrected Version
# ==============================================================================

# ------------------------------------------------------------------------------
# Metadata & Paths
# ------------------------------------------------------------------------------
name = cub3d
src_dir = src
obj_dir = obj
lib_dir = lib

# Sub-library paths - CORRECTED
libft_path  = $(lib_dir)/libft/libft
printf_path = $(lib_dir)/libft/ft_printf
gnl_path    = $(lib_dir)/libft/gnl
mlx_path    = $(lib_dir)/MLX42
mlx_build   = $(mlx_path)/build

# ------------------------------------------------------------------------------
# File Lists
# ------------------------------------------------------------------------------
src_files = main.c \
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

srcs = $(addprefix $(src_dir)/,$(src_files))
objs = $(addprefix $(obj_dir)/,$(src_files:.c=.o))
deps = $(objs:.o=.d)

# Sub-library targets - CORRECTED
libft_a  = $(libft_path)/libft.a
printf_a = $(printf_path)/libftprintf.a
gnl_a    = $(gnl_path)/libgnl.a
mlx_a    = $(mlx_build)/libmlx42.a

# ------------------------------------------------------------------------------
# Configuration (Build Type & Toolchain)
# ------------------------------------------------------------------------------
BUILD_TYPE ?= release

cc = cc
cflags = -Wall -Wextra -Werror -MMD -MP

# Includes - CORRECTED paths
includes = -I inc -I $(mlx_path)/include -I $(libft_path)

# Library linking - CORRECTED paths
ldflags = -L$(mlx_build) -lmlx42 -lglfw -ldl -pthread -lm \
		  -L$(libft_path) -lft \
		  -L$(printf_path) -lftprintf \
		  -L$(gnl_path) -lgnl

# Initialize mlx_cmake_flags
mlx_cmake_flags =

# Conditional flags based on BUILD_TYPE
ifeq ($(BUILD_TYPE), debug)
	cflags += -g3 -O0
else ifeq ($(BUILD_TYPE), asan)
	cflags += -fsanitize=address -fno-omit-frame-pointer -g3
	mlx_cmake_flags = -DCMAKE_C_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g3"
else ifeq ($(BUILD_TYPE), ubsan)
	cflags += -fsanitize=undefined -fno-sanitize-recover=all -g3
	mlx_cmake_flags = -DCMAKE_C_FLAGS="-fsanitize=undefined -fno-sanitize-recover=all -g3"
else ifeq ($(BUILD_TYPE), combined)
	cflags += -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all -g3
	mlx_cmake_flags = -DCMAKE_C_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all -g3"
else
	cflags += -O2
endif

# ------------------------------------------------------------------------------
# Main Targets
# ------------------------------------------------------------------------------
all: $(name)

$(name): $(objs) $(libft_a) $(printf_a) $(gnl_a) $(mlx_a)
	@echo "Linking $(name) ($(BUILD_TYPE) build)..."
	@$(cc) $(objs) $(ldflags) -o $(name)

# ------------------------------------------------------------------------------
# Build Rules
# ------------------------------------------------------------------------------
$(obj_dir)/%.o: $(src_dir)/%.c | $(obj_dir)
	@echo "Compiling $< ($(BUILD_TYPE) build)..."
	@$(cc) $(cflags) -c $< -o $@ $(includes)

$(obj_dir):
	@mkdir -p $(obj_dir)

-include $(deps)

# ------------------------------------------------------------------------------
# Dependency Build Handling
# ------------------------------------------------------------------------------
$(libft_a):
ifeq ($(BUILD_TYPE), release)
	@$(MAKE) -s -C $(libft_path)
else
	@$(MAKE) -s -C $(libft_path) CC="$(cc) $(cflags)"
endif

$(printf_a):
	@$(MAKE) -s -C $(printf_path)

$(gnl_a):
	@$(MAKE) -s -C $(gnl_path)

$(mlx_a):
	@echo "Building MLX42 ($(BUILD_TYPE) build)..."
	@mkdir -p $(mlx_build)
	@cmake -S $(mlx_path) -B $(mlx_build) $(mlx_cmake_flags) -Wno-dev >/dev/null 2>&1
	@$(MAKE) -s -C $(mlx_build) >/dev/null 2>&1

# ------------------------------------------------------------------------------
# Cleanup & Other Utility Targets
# ------------------------------------------------------------------------------
clean:
	@rm -rf $(obj_dir)
	@echo "Cleaned object files."

fclean: clean full-clean
	@rm -f $(name)
	@echo "Cleaned executable."

full-clean:
	@rm -rf $(mlx_build)
	@$(MAKE) -s -C $(libft_path) fclean >/dev/null 2>&1 || true
	@$(MAKE) -s -C $(printf_path) fclean >/dev/null 2>&1 || true
	@$(MAKE) -s -C $(gnl_path) fclean >/dev/null 2>&1 || true
	@echo "Fully cleaned dependencies."

re: fclean all

full-clean: fclean
	@rm -rf $(mlx_build)
	@$(MAKE) -s -C $(libft_path) fclean >/dev/null 2>&1 || true
	@$(MAKE) -s -C $(printf_path) fclean >/dev/null 2>&1 || true
	@$(MAKE) -s -C $(gnl_path) fclean >/dev/null 2>&1 || true
	@echo "Fully cleaned dependencies."

# ------------------------------------------------------------------------------
# Testing Helpers
# ------------------------------------------------------------------------------
test: all
	@echo "Running tests..."
	@./$(name) maps/valid/simple.cub

vtest:
	@echo "Running Valgrind test (debug build)..."
	@$(MAKE) BUILD_TYPE=debug
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(name) maps/valid/simple.cub

asan-test:
	@echo "Running ASan test..."
	@$(MAKE) BUILD_TYPE=asan
	@ASAN_OPTIONS=verbosity=1:abort_on_error=1 ./$(name) maps/valid/simple.cub

.PHONY: all clean fclean re full-clean test vtest asan-test