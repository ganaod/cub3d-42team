# simple.cub

. map structure: 5x5 grid, enclosed rectangular room
. spawn position: (1,1), facing north (clouds)
. floor: rgb(220,100,0) - orange/brown
. ceiling: rgb(0,0,0) - black
. purpose: minimal test case for basic raycasting functionality

# island.cub

. map structure: 11x11 grid
. spawn position: (5,5), facing south (mountain)
. floor: rgb(245,222,179) - wheat/soft warm beige
. ceiling: rgb(135,206,235) - sky blue
. purpose: medium-sized test for natural-feeling colours, open spaces

# maze.cub

. map structure: 15x11 grid, enclosed maze with multiple corridors
. spawn position: (5,9), facing east (starry sky)
. floor: rgb(85,107,47) - dark olive green
. ceiling: rgb(188,143,143) - rosy brown
. purpose: navigation testing with multiple pathways and dead ends

# crazy.cub

. map structure: 31x27 grid, complex nested maze with concentric patterns
. spawn position: (16,13), facing north (clouds)
. floor: rgb(255,0,255) - magenta
. ceiling: rgb(0,255,0) - green
. purpose: stress testing raycaster performance and complex geometry rendering

# gallery.cub
. map structure: 17x13 grid, central hall with radiating corridors
. spawn position: (8,6), facing west (sunset)
. floor: rgb(139,119,101) - warm taupe
. ceiling: rgb(205,180,158) - soft peach
. purpose: testing long sight lines and distance rendering precision