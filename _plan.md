[x] shared github repo

[] andere mlx lub? blohrer schaut dazu


[] discuss data structs - do we want to use the ones currently in .h ?






# cub3D Project Division Strategy 

## RESPONSIBILITY SPLIT

### Person A: Pure Mathematics
Files: render.c
Focus: Ray casting mathematics only

Responsibilities:
. DDA algorithm implementation
. Ray-wall intersection calculations
. Wall height calculations from distance  
. Texture coordinate mapping
. Color determination for each pixel

Key Functions:
```c
void render_frame(void);              // Main rendering loop
double cast_ray(int screen_x);        // Ray-wall intersection  
int calculate_wall_height(double dist); // Distance to screen height
int get_texture_color(int wall_face, int tex_x, int tex_y); // Texture sampling
```

### Person B: System Infrastructure  
Files: map.c, main.c, input.c, utils.c, cleanup.c, graphics.c
Focus: Everything except ray casting math

Responsibilities:
. .cub file parsing and validation
. Program lifecycle management
. MLX initialization and graphics setup
. Screen buffer operations and pixel drawing
. Keyboard handling and player movement
. Memory management and error handling
. String operations and utility functions

Key Functions:
```c
int load_map(char *filename);         // Parse .cub file
void init_graphics_system(void);      // MLX setup
void put_pixel(int x, int y, int color); // Screen buffer manipulation
void clear_buffer(void);              // Reset screen
void present_frame(void);             // Display to window
void handle_input(void);              // Process keyboard events
void update_player(void);             // Move player based on input
void cleanup_all_resources(void);     // Memory cleanup
```

## SHARED DATA STRUCTURES

Player State:
```c
typedef struct {
    double x, y;           // World position
    double dir_x, dir_y;   // View direction
    double plane_x, plane_y; // Camera plane for FOV
} t_player;
```

Map Data:
```c
typedef struct {
    int *grid;             // 2D collision array
    int width, height;     // Map dimensions  
    char *texture_paths[4]; // Wall textures [N,S,E,W]
    int floor_color;       // RGB floor color
    int ceiling_color;     // RGB ceiling color
} t_map;
```

Graphics Context:
```c
typedef struct {
    void *mlx_ptr;         // MLX library pointer
    void *window_ptr;      // Window handle
    void *image_ptr;       // Screen buffer
    char *image_data;      // Raw pixel data
    int screen_width;      // Display width
    int screen_height;     // Display height
} t_graphics;
```

Global State:
```c
extern t_game g_game;

typedef struct {
    t_player player;       // Player state
    t_map map;            // Map data
    t_graphics graphics;   // Graphics context
    int running;          // Game loop flag
} t_game;
```

## INTERFACE CONTRACTS

Person A Provides:
```c
void render_frame(void);  // Calculates colors for entire screen
```

Person B Provides:  
```c
int load_map(char *filename);    // Returns 1=success, 0=failure
void init_graphics_system(void); // Sets up MLX and screen buffer
void put_pixel(int x, int y, int color); // Draws pixel to buffer
void clear_buffer(void);         // Clears screen buffer
void present_frame(void);        // Displays buffer to window
void handle_input(void);         // Processes keyboard input
void update_player(void);        // Updates player position/rotation
void cleanup_all_resources(void); // Frees all allocated memory
```

## INTEGRATION FLOW

Data Flow:
. Person B: .cub file -> map data
. Person B: keyboard input -> player updates  
. Person A: map data + player -> pixel colors
. Person B: pixel colors -> screen display

Main Loop:
```c
while (running) {
    handle_input();    // Person B
    update_player();   // Person B  
    render_frame();    // Person A calls Person B's put_pixel()
    present_frame();   // Person B
}
```

## DEVELOPMENT PHASES

Phase 1: Parallel Development
. Person A: Implement ray casting with simple test map
. Person B: Implement .cub parsing and MLX setup

Phase 2: Integration
. Person A calls Person B's pixel functions
. Test with simple rectangular maps

Phase 3: Feature Completion  
. Person A: Add texture mapping and optimizations
. Person B: Add full input handling and error management

## KEY ADVANTAGES

Clean Separation:
. Person A: Pure mathematics and algorithms
. Person B: All system interaction and infrastructure

Minimal Dependencies:
. Person A only needs: put_pixel(), map data, player data
. Person B only needs: render_frame() to be called each frame

Focused Expertise:
. Person A masters raycasting mathematics
. Person B masters MLX library and system programming