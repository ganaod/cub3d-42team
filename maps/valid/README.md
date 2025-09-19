# simple.cub

map structure: 5x5 grid, enclosed rectangular room
spawn position: (1,1), facing north
spawn texture: clouds (north wall straight ahead)

colours:
- floor: rgb(220,100,0) - orange/brown  
- ceiling: rgb(0,0,0) - black

grid:
11111
1n001
10001  
10001
11111

purpose: minimal test case for basic raycasting functionality



# island.cub

map structure: 11x11 grid
spawn position: (5,5), facing south
spawn texture: forest mural (south wall straight ahead)

colours:
floor: rgb(245,222,179) – wheat / soft warm beige
ceiling: rgb(135,206,235) – sky blue, soft and calming

purpose: medium-sized test for natural-feeling colors, open spaces




# maze.cub

map structure: 15x11 grid, enclosed maze with multiple corridors
spawn position: (5,9), facing east
spawn texture: starry sky (east wall straight ahead)

colours:
- floor: rgb(85,107,47) - dark olive green
- ceiling: rgb(135,206,250) - light sky blue

purpose: navigation testing with multiple pathways and dead ends




# crazy.cub

map structure: 31x27 grid, complex nested maze with concentric patterns
spawn position: (16,13), facing north  
spawn texture: clouds (north wall straight ahead)

colours:
- floor: rgb(255,0,255) - magenta
- ceiling: rgb(0,255,0) - green

purpose: stress testing raycaster performance and complex geometry rendering