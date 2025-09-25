/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:56:12 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/25 14:59:26 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* dda setup: transform ray dir > grid traversal params

purpose:
. convert continous ray dir to discrete stepping params
. pre-calculate distances to avoid runtime computation
. determine initial boundary distances for efficient traversal

insight:
grid spacing is uniform & ray's angle stays const:
calculate step distances once,
reuse throughout traversal.
this is the optimisation that makes dda superior
to naive ray stepping approaches
this delta_dist is a fixed constant for the entire ray's journey */

#include "../inc/render.h"

/* DDA SETUP INITIALIZATION
transform ray direction → DDA stepping params

purpose:
. convert continuous ray direction to discrete grid traversal
. calculate delta distances (distance per grid step)
. initialise stepping directions and boundary distances
. prepare state for efficient grid traversal */
void	setup_dda_vars(t_game *g, double ray_dir_x, double ray_dir_y,
		t_dda_state *state)
{
	state->map_x = (int)g->player.world_pos_x;
	state->map_y = (int)g->player.world_pos_y;
	state->delta_dist_x = fabs(1.0 / ray_dir_x);
	state->delta_dist_y = fabs(1.0 / ray_dir_y);
	state->world_ray_dir_x = ray_dir_x;
	state->world_ray_dir_y = ray_dir_y;
	setup_x_step(g, ray_dir_x, state);
	setup_y_step(g, ray_dir_y, state);
	state->wall_intersection_found = 0;
}

/* x-axis stepping configuration
initial pre-computation
calculate step direction and distance to first X boundary

Mathematical setup:
- Determine stepping direction: +1 (East) or -1 (West)
- Calculate distance from player to next X grid line
- Handles both positive and negative ray directions */
void	setup_x_step(t_game *g, double ray_dir_x, t_dda_state *state)
{
	double	player_x;
	double	dist;

	player_x = g->player.world_pos_x;
	if (ray_dir_x < 0)
	{
		state->step_x = -1;
		dist = (player_x - state->map_x) * state->delta_dist_x;
		state->world_dist_to_next_boundary_x = dist;
	}
	else
	{
		state->step_x = 1;
		dist = (state->map_x + 1.0 - player_x) * state->delta_dist_x;
		state->world_dist_to_next_boundary_x = dist;
	}
}

/* Y-AXIS STEPPING CONFIGURATION
calculate step direction and distance to first Y boundary

setup:
. determine stepping direction: +1 (South) or -1 (North)
. calculate distance from player to next Y grid line
. handles both positive and negative ray directions */
void	setup_y_step(t_game *g, double ray_dir_y, t_dda_state *state)
{
	double	player_y;
	double	dist;

	player_y = g->player.world_pos_y;
	if (ray_dir_y < 0)
	{
		state->step_y = -1;
		dist = (player_y - state->map_y) * state->delta_dist_y;
		state->world_dist_to_next_boundary_y = dist;
	}
	else
	{
		state->step_y = 1;
		dist = (state->map_y + 1.0 - player_y) * state->delta_dist_y;
		state->world_dist_to_next_boundary_y = dist;
	}
}
