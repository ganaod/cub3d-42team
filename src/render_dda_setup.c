/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:01:49 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/05 17:02:19 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* initialise
calculate all mathematical setup vars
transform ray dir > DDA stepping params */
void	setup_dda_vars(double ray_dir_x, double ray_dir_y, t_dda_state *state)
{
	state->map_x = (int)g_game.player.pos_x;
	state->map_y = (int)g_game.player.pos_y;
	state->delta_dist_x = fabs(1.0 / ray_dir_x);
	state->delta_dist_y = fabs(1.0 / ray_dir_y);
	state->ray_dir_x = ray_dir_x;
	state->ray_dir_y = ray_dir_y;
	setup_x_axis_stepping(ray_dir_x, state);
	setup_y_axis_stepping(ray_dir_y, state);
	state->wall_hit = 0;
}

/* stepping set up
prepares:
. which dir to step thru grid (step_x, step_y)
. how far to 1st grid boundary (side_dist_x, side_dist_y) 

enables core dda loop:
if x boundary closer: step in x dir ...*/
static void	setup_x_axis_stepping(double ray_dir_x, t_dda_state *state)
{
	if (ray_dir_x < 0)
	{
		state->step_x = -1;
		state->side_dist_x = (g_game.player.pos_x - state->map_x)
							* state->delta_dist_x;
	}
	else
	{
		state->step_x = 1;
		state->side_dist_x = (state->map_x + 1.0 - g_game.player.pos_x)
							* state->delta_dist_x;
	}
}

static void	setup_y_axis_stepping(double ray_dir_y, t_dda_state *state)
{
	if (ray_dir_y < 0)
	{
		state->step_y = -1;
		state->side_dist_y = (g_game.player.pos_y - state->map_y)
							* state->delta_dist_y;
	}
	else
	{
		state->step_y = 1;
		state->side_dist_y = (state->map_y + 1.0 - g_game.player.pos_y)
							* state->delta_dist_y;
	}
}