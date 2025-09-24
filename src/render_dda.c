#include "../inc/render.h"
#include <math.h> // if needed elsewhere

/* Cast a single ray and return wall hit info (no globals) */
t_ray_result	cast_ray_to_wall(t_game *g, double world_ray_dir_x, double world_ray_dir_y)
{
	t_dda_state	dda_state;
	t_ray_result	res;

	/* init DDA from player pos + ray dir */
	setup_dda_vars(g, world_ray_dir_x, world_ray_dir_y, &dda_state);

	/* walk the grid until we hit a wall */
	execute_dda_traversal(g, &dda_state, &res.world_wall_side);

	/* perpendicular distance (prevents fisheye) */
	res.world_perpendicular_distance =
		calculate_wall_distance(g, &dda_state, res.world_wall_side);

	/* precise world intersection (for texture mapping) */
	res.world_intersection_x = g->player.world_pos_x
		+ res.world_perpendicular_distance * world_ray_dir_x;
	res.world_intersection_y = g->player.world_pos_y
		+ res.world_perpendicular_distance * world_ray_dir_y;

	/* which face (N/E/S/W or similar) for texture selection */
	res.world_wall_face = determine_intersected_wall_face(g, &res);

	return (res);
}

/* Step to the nearest next grid boundary until a wall cell is found */
void	execute_dda_traversal(t_game *g, t_dda_state *s, int *world_wall_side)
{
	(void)g; /* only used for map access below; keep if you later swap to map_cell() */

	while (!s->wall_intersection_found)
	{
		if (s->world_dist_to_next_boundary_x < s->world_dist_to_next_boundary_y)
		{
			s->world_dist_to_next_boundary_x += s->delta_dist_x;
			s->map_x += s->step_x;
			*world_wall_side = VERTICAL_WALL;
		}
		else
		{
			s->world_dist_to_next_boundary_y += s->delta_dist_y;
			s->map_y += s->step_y;
			*world_wall_side = HORIZONTAL_WALL;
		}
		/* wall check */
		if (g->map.grid[s->map_y * g->map.width + s->map_x] == CELL_WALL)
			s->wall_intersection_found = 1;
	}
}

/* Perpendicular distance to the wall from the player along the ray */
double	calculate_wall_distance(t_game *g, t_dda_state *s, int world_wall_side)
{
	(void)g; /* not needed for this calculation; keep for signature parity */

	if (world_wall_side == VERTICAL_WALL)
		return (s->world_dist_to_next_boundary_x - s->delta_dist_x);
	else
		return (s->world_dist_to_next_boundary_y - s->delta_dist_y);
}
