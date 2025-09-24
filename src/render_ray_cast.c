/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray_cast.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:48:28 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/24 14:55:28 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* core responsibility: screen coordinate → world ray vector transformations */

#include "../inc/render.h"

/* what: create natural peripheral vision in 3D game
how: transform ray caster from 1D scanner > 3D vision simulator

1. map screen column index to normalised FOV position [-1, +1]
	. leftmost col = -1, centre - 0, rightmost = +1
	. this offset encodes relative angular deviation
2. compute ray dir vector for this column:
world_ray_dir = dir + offset * plane
	. dir: forward-facing vector
	. plane: perpendicular vector spanning FOV width
	. offset ∈ [-1,1], scales plane vector
	. result: parametric dir vector for ray in world space
3. viual effect:
	. without offset: all rays point forwards > tunnel vision
	. with offset: rays sweep l>r > human-like peripheral vision
notes on plane:
	. normalised to FOV width (not dist from player)
	. acts as a helper for generating angular deviation
	. scaling offset * plane produces angular spread across screen columns
top-down view of rays from player (P):
        / ray offset = +1 (right)
       /
  P -> dir (forward)
       \
        \ ray offset = -1 (left) */
void	calculate_ray_direction(t_game *g, int screen_column_x,
	double *world_ray_direction_x, double *world_ray_direction_y)
{
	double	fov_space_camera_plane_offset;

	fov_space_camera_plane_offset
		= (SCREEN_TO_FOV_SCALE_FACTOR * screen_column_x)
		/ (double)g->graphics.screen_width - FOV_CENTER_OFFSET;
	*world_ray_direction_x = g->player.world_dir_x
		+ fov_space_camera_plane_offset * g->player.world_camera_plane_x;
	*world_ray_direction_y = g->player.world_dir_y
		+ fov_space_camera_plane_offset * g->player.world_camera_plane_y;
}

/* no peripheral sweep → all columns show the same forward view.
visual effect: telescope-like, extremely narrow, boring “tunnel” view */
// void	calculate_ray_direction_tunnel_vision(int screen_column_x,
// 		double *world_ray_direction_x, double *world_ray_direction_y)
// {
// 	(void)screen_column_x;
// 	*world_ray_direction_x = g->player.world_dir_x;
// 	*world_ray_direction_y = g->player.world_dir_y;
// }

/* why? determine which compass dir wall face was hit by a ray
in: t_ray_result: world coords of intersection + wall intersection (vert/horz)
out: WALL_N/S/E/W

how? 1. check wall orientation (vert/horz)
2. compare intersection coords with player pos
	. vert walls: x-coord
		intersection_x > player_x : E
	. horz walls: y-coord
		intersection_y > player_y : S
3. ret compass dir const for texture selection */
int	determine_intersected_wall_face(t_game *g, t_ray_result *wall_intersection_data)
{
	if (wall_intersection_data->world_wall_side == VERTICAL_WALL)
	{
		if (wall_intersection_data->world_intersection_x
			> g->player.world_pos_x)
			return (WALL_EAST);
		else
			return (WALL_WEST);
	}
	else
	{
		if (wall_intersection_data->world_intersection_y
			> g->player.world_pos_y)
			return (WALL_SOUTH);
		else
			return (WALL_NORTH);
	}
}
