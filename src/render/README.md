## math.c

Parametric line intersection with discrete grid
ray casting & DDA algos

## projection.c

3D world coordinates → 2D screen coordinates
screen space projection calculations

## texture.c

Surface parameterization and texture sampling
texture mapping & sampling

## screen_buffer.c

Rasterization and pixel buffer operations
direct screen buffer ops



Justification for separation:
Ray Math: Pure geometric algorithms - grid traversal, intersection testing
Projection: Perspective mathematics - similar triangles, distance scaling
Texture: Surface parameterization - UV coordinate mapping, image sampling
Screen Buffer: Direct memory operations - pixel manipulation, buffer management
Each file encapsulates one mathematical domain with minimal cross-dependencies

Dependency Flow: 
ray_math.c → projection.c → texture.c → screen_buffer.c







render/ray_math.c
Mathematical Domain: Parametric line intersection with discrete grid
cvoid calculate_ray_direction(int screen_x, double *ray_dir_x, double *ray_dir_y);
double cast_ray_to_wall(double ray_dir_x, double ray_dir_y, int *wall_side);
int get_wall_face_hit(/* DDA intersection data */);
render/projection.c
Mathematical Domain: 3D world coordinates → 2D screen coordinates
cint calculate_screen_wall_height(double wall_distance);
void calculate_wall_boundaries(int wall_height, int *wall_start, int *wall_end);
render/texture.c
Mathematical Domain: Surface parameterization and texture sampling
ctexture_image *get_texture_for_direction(int wall_direction);
int calculate_texture_x(double wall_hit_pos, int wall_side);
int calculate_texture_y(int screen_y, int wall_start, int wall_height);
int sample_texture_pixel(texture_image *tex, int tex_x, int tex_y);
render/screen_buffer.c
Mathematical Domain: Rasterization and pixel buffer operations
cvoid clear_screen_buffer(void);
void put_pixel(int x, int y, int color);
void present_frame_to_screen(void);