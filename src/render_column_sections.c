/* screen with its 3 conceptual zones:

                  ┌───────────────────────────────────────────────┐
                  │                                               │
                  │              CEILING (Rays above              │
                  │            the horizontal centreline)         │
                  │                                               │
Screen-Y = wall_start_y_pixel ├───────────────────────────────────────────────┤  
                  │                                               │
                  │                 WALL SECTION                  │
                  │              (Ray intersection               │
                  │               determines height)              │
                  │                                               │
Screen-Y = wall_end_y_pixel   ├───────────────────────────────────────────────┤
                  │                                               │
                  │                 FLOOR (Rays below             │
                  │            the horizontal centreline)         │
                  │                                               │
                  └───────────────────────────────────────────────┘

re. texture, from subject:
	"display different wall textures"
	"set the floor and ceiling to two different colours"

hence 
1 helper per section, texturing
only in render_wall_section() */

#include "../inc/cub3d.h"

/* render ceiling pixels above wall */
void	render_ceiling_section(int screen_column_x, int wall_start_y_pixel)
{
	int	current_pixel_y;

	current_pixel_y = 0;
	while (current_pixel_y < wall_start_y_pixel)
	{
		put_pixel(screen_column_x, current_pixel_y, g_game.map.ceiling_color);
		current_pixel_y++;
	}
}

/* render wall texture pixels */
void	render_wall_section(int screen_column_x, int wall_start_y_pixel,
						int wall_end_y_pixel, t_ray_result *wall_hit_data)
{
	int					current_pixel_y;
	uint32_t			pixel_color;
	t_texture_context	texture_mapping_info;

	texture_mapping_info.world_wall_face = wall_hit_data->world_wall_face;
	texture_mapping_info.world_wall_intersection_x = wall_hit_data->world_intersection_x;
	texture_mapping_info.world_wall_intersection_y = wall_hit_data->world_intersection_y;
	texture_mapping_info.screen_wall_height = wall_end_y_pixel - wall_start_y_pixel;
	current_pixel_y = wall_start_y_pixel;
	while (current_pixel_y < wall_end_y_pixel)
	{
		pixel_color = get_wall_texture_color(&texture_mapping_info,
						current_pixel_y);
		put_pixel(screen_column_x, current_pixel_y, pixel_color);
		current_pixel_y++;
	}
}

// DEMO: replace get_wall_texture_colour() call with:
// texture_colour = get_wall_hardcoded_color(wall_direction)

/* render floor pixels below wall */
void	render_floor_section(int screen_x, int wall_end_y)
{
	int	y;
	
	y = wall_end_y;
	while (y < g_game.graphics.screen_height)
	{
		put_pixel(screen_x, y, g_game.map.floor_colour);
		y++;
	}
}
