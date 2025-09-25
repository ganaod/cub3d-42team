/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:16:57 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/25 10:02:13 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/* ⭐⭐ INCLUDES */
/* system */
# include <fcntl.h>
# include <stdint.h> /* <-- for uint32_t */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

/* math */
# include <math.h>

/* external */
# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/ft_printf/ft_printf.h"
# include "../lib/gnl/get_next_line.h"
# include "../lib/libft/libft.h"

/* ⭐⭐ DEFINES */
/* ⭐ SYSTEM ARCHITECTURE */
/* framebuffer display resolution */
# define DEFAULT_WIDTH 1024
# define DEFAULT_HEIGHT 768

/* absolute maximum limits (memory protection) */
# define MAX_RENDER_WIDTH 1600
# define MAX_RENDER_HEIGHT 900

/* exit codes */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

/* ⭐ MATHEMATICAL FOUNDATION */
/* (fixed) field of view (FoV) configuration */
# define FOV_DEGREES 60.0

/* SCREEN > FOV COORDINATE TRANSFORMATION */
# define SCREEN_TO_FOV_SCALE_FACTOR 2.0
# define FOV_CENTER_OFFSET 1.0

/* wall face directions: absolute world coords */
# define WALL_NORTH 0
# define WALL_SOUTH 1
# define WALL_WEST 2
# define WALL_EAST 3

/* player direction codes: relative to actor orientation */
# define DIR_N 0
# define DIR_S 1
# define DIR_W 2
# define DIR_E 3

/* ⭐ WORLD DOMAIN PRIMITIVES */
/* map cell types */
typedef enum e_cell
{
	CELL_VOID = -1,
	CELL_EMPTY = 0,
	CELL_WALL = 1
}					t_cell;

/* ⭐ PARSER STATE FLAGS */
enum
{
	HDR_NO = (1 << 0),
	HDR_SO = (1 << 1),
	HDR_WE = (1 << 2),
	HDR_EA = (1 << 3),
	HDR_F = (1 << 4),
	HDR_C = (1 << 5),
};

/* ⭐⭐ STRUCTS */
/* PRIMITIVE DATA CONTAINERS */
typedef struct s_texture_image
{
	mlx_image_t		*mlx_image;
	uint32_t		*pixels;
	int				image_width;
	int				image_height;
}					t_texture_image;

/* ⭐ WORLD ENTITY REPRESENTATION */
/* player state */
typedef struct s_player
{
	double			world_pos_x;
	double			world_pos_y;
	double			world_dir_x;
	double			world_dir_y;
	double			world_camera_plane_x;
	double			world_camera_plane_y;
}					t_player;

/* map configuration */
typedef struct s_map
{
	int				*grid;
	int				width;
	int				height;
	char			*texture_paths[4];
	uint32_t		floor_color;
	uint32_t		ceiling_color;
	char			*first_map_line;
	t_texture_image	wall_textures[4];
}					t_map;

/* ⭐ SYSTEM INTERFACES */
/* graphics system */
typedef struct s_graphics
{
	mlx_t			*mlx;
	mlx_image_t		*frame;
	int				screen_width;
	int				screen_height;
}					t_graphics;

/* ⭐ UTILITIES */
typedef struct s_ffctx
{
	const t_map		*m;
	char			*vis;
	int				*q;
	int				w;
	int				h;
	int				head;
	int				tail;
}					t_ffctx;

/* parser contexts */
typedef struct s_texinfo
{
	char			**slot;
	const char		*line;
	int				j;
	int				bit;
	int				*flags;
}					t_texinfo;

/* ⭐ ORCHESTRATION */
/* game state */
typedef struct s_game
{
	t_player		player;
	t_map			map;
	t_graphics		graphics;
	int				running;
	double			movement_speed;
	double			rotation_speed;
	double			time_prev;
	double			time_now;
}					t_game;

/* ⭐⭐ FUNCTION PROTOTYPES */
/* ================== PARSE_UTILS ================== */
int					skip_ws(const char *s, int i);
int					parse_u8_component(const char *s, int *i, int *out);
int					expect_comma(const char *s, int *i);
int					parse_rgb_triplet(const char *s, uint32_t *out_rgb);
int					open_rdonly(const char *path);

/* ================== PARSE_HEADER ================== */
int					set_texture_field(char **dst_path, const char *after_key);
int					set_color_field(uint32_t *dst_rgb, const char *after_key,
						int *was_set);
int					parse_header_line(t_map *m, const char *line, int *flags);
int					match2(const char *s, int i, char a, char b);
int					set_tex_entry(char **slot, const char *after_key, int bit,
						int *flags);
int					parse_header_texture(t_map *m, const char *line, int i,
						int *flags);
int					parse_header_color(t_map *m, const char *line, int i,
						int *flags);

/* ================== PARSE_HEADER LINES ================== */
int					parse_header_lines(t_map *m, int fd);
int					check_texture_paths_exist(const t_map *m);

/* ================== COLLECT_MAP_LINES ================== */
int					collect_map_lines(t_map *m, int fd, char ***out_lines,
						int *out_h);
int					append_line(char ***lines_ptr, int *cap_ptr, int *h_ptr,
						char *take);
int					validate_map_line(const char *s);
void				rstrip_eol(char *s);

/* ================== NORMALIZE_MAP ================== */
int					normalize_map(char ***lines_io, int h, int *out_w);

/* ================== MAP_GRID_CELL ================== */
int					put_cell_from_char(t_map *m, t_player *pl, int idx, char c);

/* ================== MAP_GRID_FILL ================== */
int					fill_grid(t_map *m, t_player *pl, char **lines, int *count);

/* ================== MAP_GRID_BUILD ================== */
int					build_grid_from_lines(t_map *m, t_player *pl, char **lines,
						int *player_found);

/* ================== MAP_CHECK_FLOOD ================== */
void				ffctx_init(t_ffctx *c, const t_map *m, char *vis, int *q);
int					flood_from_start(t_ffctx *c, int start_idx);
int					map_has_open_border(const t_map *m);
int					border_rows_have_empty(const t_map *m, int w, int h);
int					border_cols_have_empty(const t_map *m, int w, int h);
int					cell_adjacent_to_void(const t_map *m, int x, int y);
int					has_empty_adjacent_to_void(const t_map *m);

/* ================== MAP_CHECK_UTILS ================== */
int					idx_2d_to_1d(int x, int y, int w);
int					in_bounds(int x, int y, int w, int h);
void				queue_push(int *q, int *tail, int v);
int					queue_pop(int *q, int *head, int tail, int *out);
int					load_textures_or_fail(t_game *g);

/* ================== MAP_CHECK_FLOOD ================== */
int					flood_from_border(t_ffctx *c);
int					map_is_closed(const t_map *m);

/* ================== GAME_LOOP ================== */
double				get_delta_time(t_game *g);
void				rotate_player(t_game *g, double angle);
void				try_move_player(t_game *g, double new_x, double new_y);
void				handle_input(t_game *g, double dt);
void				game_loop_tick(void *param);
void				on_close(void *param);
void				handle_exit_input(t_game *g);
void				handle_rotation_input(t_game *g, double ang);
int					map_cell(const t_map *m, int ix, int iy);

/* ================== UTILS ================== */
void				parse_error(const char *msg);
int					check_cub_extension(const char *filename);

/* ================== GRAPHICS / WINDOW ================== */
int					gfx_open_window_resizable(t_game *game, const char *title,
						int w, int h);
int					gfx_maximize_now(t_graphics *g);
int					gfx_rebuild_framebuffer(t_graphics *g, int w, int h);
void				handle_resize(int32_t w, int32_t h, void *param);

/* ================== LOAD_TEXTURE ================== */
int					load_one_texture_png(const char *path,
						t_texture_image *out);
int					load_all_wall_textures(t_map *m);

/* ================== CLEANUP ================== */
void				free_lines_array(char **lines, int h);
void				free_map(t_game *g, t_map *m);
int					cleanup_and_exit(t_game *g, int code);

/* ================== RUNTIME_INIT ================== */
void				init_runtime_defaults(t_game *g);
int					start_window(t_game *g, const char *title, int w, int h);
void				sync_player_world_fields_from_parser(t_game *g);
int					map_load_and_validate(t_game *g, const char *path);

#endif
