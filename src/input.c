/* keyboard input processing & player movement */

#include "../inc/cub3d.h"

/*
UPSTREAM RESPONSIBILITY:
- Process keyboard events from graphics system
- Translate input into player movement commands
- Update player position and orientation
- Handle collision detection during movement
*/

/*
MOVEMENT SYSTEM DESIGN:
Two types of movement:
1. LOCOMOTION: Forward/backward along view direction
2. STRAFING: Left/right perpendicular to view direction  
3. ROTATION: Turn view direction left/right

COLLISION SYSTEM:
- Check destination before moving
- Prevent movement into walls
- Allow smooth sliding along wall edges
*/

void process_input_events(void)
{
    // MOVEMENT INPUT: WASD locomotion
    if (key_is_pressed(KEY_W))
        move_player_forward();
    if (key_is_pressed(KEY_S)) 
        move_player_backward();
    if (key_is_pressed(KEY_A))
        strafe_player_left();
    if (key_is_pressed(KEY_D))
        strafe_player_right();
    
    // ROTATION INPUT: Arrow keys or QE
    if (key_is_pressed(KEY_LEFT) || key_is_pressed(KEY_Q))
        rotate_player_left();
    if (key_is_pressed(KEY_RIGHT) || key_is_pressed(KEY_E))
        rotate_player_right();
        
    // SYSTEM COMMANDS
    if (key_is_pressed(KEY_ESC))
        initiate_program_exit();
}

/*
FORWARD/BACKWARD MOVEMENT:
Move along current view direction vector.
Player faces direction of movement.
*/
void move_player_forward(void)
{
    // Calculate destination position
    double new_x = player.pos_x + (player.dir_x * movement_speed);
    double new_y = player.pos_y + (player.dir_y * movement_speed);
    
    // Collision detection before movement
    if (can_move_to_position(new_x, new_y))
    {
        player.pos_x = new_x;
        player.pos_y = new_y;
    }
    else
    {
        // Attempt sliding movement along walls
        attempt_sliding_movement(new_x, new_y);
    }
}

void move_player_backward(void)
{
    // Same as forward but negative direction
    double new_x = player.pos_x - (player.dir_x * movement_speed);
    double new_y = player.pos_y - (player.dir_y * movement_speed);
    
    if (can_move_to_position(new_x, new_y))
    {
        player.pos_x = new_x;
        player.pos_y = new_y;
    }
}

/*
STRAFING MOVEMENT:
Move perpendicular to view direction.
Player maintains facing direction while moving sideways.
*/
void strafe_player_left(void)
{
    // Calculate perpendicular direction (90 degrees left from view)
    double strafe_dir_x = -player.dir_y;  // Rotate direction 90° left
    double strafe_dir_y = player.dir_x;
    
    double new_x = player.pos_x + (strafe_dir_x * movement_speed);
    double new_y = player.pos_y + (strafe_dir_y * movement_speed);
    
    if (can_move_to_position(new_x, new_y))
    {
        player.pos_x = new_x;
        player.pos_y = new_y;
    }
}

void strafe_player_right(void)
{
    // Calculate perpendicular direction (90 degrees right from view)
    double strafe_dir_x = player.dir_y;   // Rotate direction 90° right
    double strafe_dir_y = -player.dir_x;
    
    double new_x = player.pos_x + (strafe_dir_x * movement_speed);
    double new_y = player.pos_y + (strafe_dir_y * movement_speed);
    
    if (can_move_to_position(new_x, new_y))
    {
        player.pos_x = new_x;
        player.pos_y = new_y;
    }
}

/*
ROTATION SYSTEM:
Rotate player's view direction and camera plane.
Uses rotation matrix mathematics.
*/
void rotate_player_left(void)
{
    // Rotation matrix for counter-clockwise rotation
    double old_dir_x = player.dir_x;
    double old_plane_x = player.camera_plane_x;
    
    player.dir_x = player.dir_x * cos(rotation_speed) - player.dir_y * sin(rotation_speed);
    player.dir_y = old_dir_x * sin(rotation_speed) + player.dir_y * cos(rotation_speed);
    
    player.camera_plane_x = player.camera_plane_x * cos(rotation_speed) - player.camera_plane_y * sin(rotation_speed);
    player.camera_plane_y = old_plane_x * sin(rotation_speed) + player.camera_plane_y * cos(rotation_speed);
}

void rotate_player_right(void)
{
    // Rotation matrix for clockwise rotation (negative angle)
    double old_dir_x = player.dir_x;
    double old_plane_x = player.camera_plane_x;
    
    player.dir_x = player.dir_x * cos(-rotation_speed) - player.dir_y * sin(-rotation_speed);
    player.dir_y = old_dir_x * sin(-rotation_speed) + player.dir_y * cos(-rotation_speed);
    
    player.camera_plane_x = player.camera_plane_x * cos(-rotation_speed) - player.camera_plane_y * sin(-rotation_speed);
    player.camera_plane_y = old_plane_x * sin(-rotation_speed) + player.camera_plane_y * cos(-rotation_speed);
}

/*
COLLISION DETECTION:
Check if position contains wall before allowing movement.
*/
int can_move_to_position(double x, double y)
{
    // Convert world coordinates to map grid coordinates
    int map_x = (int)x;
    int map_y = (int)y;
    
    // Check boundaries
    if (map_x < 0 || map_x >= map_width || map_y < 0 || map_y >= map_height)
        return (0);
    
    // Check for wall collision
    if (map_grid[map_y][map_x] == WALL)
        return (0);
        
    return (1);
}