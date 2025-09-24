/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:01:49 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/24 11:07:23 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* dda setup: transform ray dir > grid traversal params 

purpose:
. convert continous ray dir to discrete stepping params
. pre-calculate distances to avoid runtime computation
. determine initial boundary distances for efficient traversal

insight:
grid spacing is uniform: calculate step distances once,
reuse throughout traversal.
this is the optimisation that makes dda superior
to naive ray stepping approaches */

#include "../inc/render.h"

static void	setup_x_axis_stepping(double world_ray_dir_x, t_dda_state *dda_state);
static void	setup_y_axis_stepping(double world_ray_dir_y, t_dda_state *dda_state);

/* DDA SETUP INITIALIZATION
transform ray direction → DDA stepping params

purpose:
. convert continuous ray direction to discrete grid traversal
. calculate delta distances (distance per grid step)
. initialise stepping directions and boundary distances
. prepare state for efficient grid traversal */
void	setup_dda_vars(double world_ray_dir_x, double world_ray_dir_y, 
			t_dda_state *dda_state)
{
	dda_state->map_x = (int)g_game.player.world_pos_x;
	dda_state->map_y = (int)g_game.player.world_pos_y;
	dda_state->delta_dist_x = fabs(1.0 / world_ray_dir_x);
	dda_state->delta_dist_y = fabs(1.0 / world_ray_dir_y);
	dda_state->world_ray_dir_x = world_ray_dir_x;
	dda_state->world_ray_dir_y = world_ray_dir_y;
	setup_x_axis_stepping(world_ray_dir_x, dda_state);
	setup_y_axis_stepping(world_ray_dir_y, dda_state);
	dda_state->wall_intersection_found = 0;
}

/* X-AXIS STEPPING CONFIGURATION
calculate step direction and distance to first X boundary

Mathematical setup:
- Determine stepping direction: +1 (East) or -1 (West)
- Calculate distance from player to next X grid line
- Handles both positive and negative ray directions */
static void	setup_x_axis_stepping(double world_ray_dir_x, t_dda_state *dda_state)
{
	if (world_ray_dir_x < 0)
	{
		// Ray pointing West (-X direction)
		dda_state->step_x = -1;
		dda_state->world_dist_to_next_boundary_x = 
			(g_game.player.world_pos_x - dda_state->map_x) * dda_state->delta_dist_x;
	}
	else
	{
		// Ray pointing East (+X direction)
		dda_state->step_x = 1;
		dda_state->world_dist_to_next_boundary_x = 
			(dda_state->map_x + 1.0 - g_game.player.world_pos_x) * dda_state->delta_dist_x;
	}
}

/* Y-AXIS STEPPING CONFIGURATION
calculate step direction and distance to first Y boundary

setup:
. determine stepping direction: +1 (South) or -1 (North)
. calculate distance from player to next Y grid line
. handles both positive and negative ray directions */
static void	setup_y_axis_stepping(double world_ray_dir_y, t_dda_state *dda_state)
{
	if (world_ray_dir_y < 0)
	{
		// Ray pointing North (-Y direction)
		dda_state->step_y = -1;
		dda_state->world_dist_to_next_boundary_y = 
			(g_game.player.world_pos_y - dda_state->map_y) * dda_state->delta_dist_y;
	}
	else
	{
		// Ray pointing South (+Y direction) 
		dda_state->step_y = 1;
		dda_state->world_dist_to_next_boundary_y = 
			(dda_state->map_y + 1.0 - g_game.player.world_pos_y) * dda_state->delta_dist_y;
	}
}
