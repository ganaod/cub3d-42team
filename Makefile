# metadata
name		= cub3d
test_name	= test

# paths
src_dir		= src
obj_dir		= obj
libft_path	= lib/libft/libft
printf_path	= lib/libft/ft_printf
gnl_path	= lib/libft/gnl
mlx_path	= lib/MLX42
mlx_build	= $(mlx_path)/build

# file lists
src_files =	main.c \
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
			cleanup.c

srcs		= $(addprefix $(src_dir)/,$(src_files))
objs		= $(addprefix $(obj_dir)/,$(src_files:.c=.o))
deps		= $(objs:.o=.d)

libft_a		= $(libft_path)/libft.a
printf_a	= $(printf_path)/libftprintf.a
gnl_a		= $(gnl_path)/libgnl.a
mlx_a		= $(mlx_build)/libmlx42.a

# toolchain
cc			= cc
# base flags - production build
cflags		= -Wall -Wextra -Werror -MMD -MP
includes	= -I include -I $(mlx_path)/include -I $(libft_path)/include
ldflags		= -L$(libft_path) -lft \
			  -L$(printf_path) -lftprintf \
			  -L$(gnl_path) -lgnl \
			  -L$(mlx_build) -lmlx42 -lglfw -ldl -pthread -lm

# ==== MAIN TARGETS ====

# production build
all: $(name)

# valgrind-compatible build - no sanitizers, max debug info
debug: cflags += -g3 -O0
debug: $(name)

# addresssanitizer - detects memory errors, requires compatible dependencies
asan: cflags += -fsanitize=address -fno-omit-frame-pointer -g3
asan: rebuild-deps-asan $(name)

# undefined behavior sanitizer - detects undefined operations
ubsan: cflags += -fsanitize=undefined -fno-sanitize-recover=all -g3
ubsan: rebuild-deps-ubsan $(name)

# combined sanitizers - maximum error detection
combined: cflags += -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all -g3
combined: rebuild-deps-combined $(name)

# ==== BUILD RULES ====

$(name): $(objs) $(libft_a) $(printf_a) $(gnl_a) $(mlx_a)
	@$(cc) $(objs) $(ldflags) -o $(name)

$(obj_dir)/%.o: $(src_dir)/%.c | $(obj_dir)
	@$(cc) $(cflags) -c $< -o $@ $(includes)

$(obj_dir):
	@mkdir -p $(obj_dir)

-include $(deps)

# ==== DEPENDENCY HANDLING ====

# normal dependency builds - no special flags
$(libft_a):
	@$(MAKE) -s -C $(libft_path) >/dev/null 2>&1

$(printf_a):
	@$(MAKE) -s -C $(printf_path) >/dev/null 2>&1

$(gnl_a):
	@$(MAKE) -s -C $(gnl_path) >/dev/null 2>&1

$(mlx_a):
	@mkdir -p $(mlx_build)
	@cmake -S $(mlx_path) -B $(mlx_build) -Wno-dev >/dev/null 2>&1
	@$(MAKE) -s -C $(mlx_build) >/dev/null 2>&1

# ==== ANALYSIS BUILD DEPENDENCY MANAGEMENT ====

# force rebuild all dependencies with sanitizer flags
rebuild-deps-asan: clean-all-deps
	@$(MAKE) -s $(libft_a) CFLAGS="-fsanitize=address -fno-omit-frame-pointer -g3"
	@$(MAKE) -s $(printf_a) CFLAGS="-fsanitize=address -fno-omit-frame-pointer -g3"
	@$(MAKE) -s $(gnl_a) CFLAGS="-fsanitize=address -fno-omit-frame-pointer -g3"
	@$(MAKE) -s $(mlx_a)  # warning: mlx42 uses cmake, may not inherit sanitizer flags

rebuild-deps-ubsan: clean-all-deps
	@$(MAKE) -s $(libft_a) CFLAGS="-fsanitize=undefined -fno-sanitize-recover=all -g3"
	@$(MAKE) -s $(printf_a) CFLAGS="-fsanitize=undefined -fno-sanitize-recover=all -g3"
	@$(MAKE) -s $(gnl_a) CFLAGS="-fsanitize=undefined -fno-sanitize-recover=all -g3"
	@$(MAKE) -s $(mlx_a)

rebuild-deps-combined: clean-all-deps
	@$(MAKE) -s $(libft_a) CFLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all -g3"
	@$(MAKE) -s $(printf_a) CFLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all -g3"
	@$(MAKE) -s $(gnl_a) CFLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all -g3"
	@$(MAKE) -s $(mlx_a)

# ==== CLEANUP ====

clean:
	@rm -rf $(obj_dir)

# clean only dependencies - keeps your object files
clean-deps:
	@rm -rf $(mlx_build)
	@$(MAKE) -s -C $(libft_path) clean >/dev/null 2>&1 || true
	@$(MAKE) -s -C $(printf_path) clean >/dev/null 2>&1 || true
	@$(MAKE) -s -C $(gnl_path) clean >/dev/null 2>&1 || true

# clean everything including dependencies
clean-all-deps: clean-deps
	@$(MAKE) -s -C $(libft_path) fclean >/dev/null 2>&1 || true
	@$(MAKE) -s -C $(printf_path) fclean >/dev/null 2>&1 || true
	@$(MAKE) -s -C $(gnl_path) fclean >/dev/null 2>&1 || true

fclean: clean clean-all-deps
	@rm -f $(name) $(test_name)

re: fclean all

# ==== ANALYSIS HELPERS ====

# quick valgrind test with debug build
vtest: debug
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(name) maps/valid/simple.cub

# quick asan test (if build succeeds)
atest: asan
	@ASAN_OPTIONS=verbosity=1:abort_on_error=1 ./$(name) maps/valid/simple.cub

.PHONY: all debug asan ubsan combined clean clean-deps clean-all-deps fclean re rebuild-deps-asan rebuild-deps-ubsan rebuild-deps-combined vtest atest