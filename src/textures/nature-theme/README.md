this is just me playing around, having some fun, getting to know the texture-related parts of the program

I generally like nature-themed imagery, hence:

	north: sunset at ocean
	south: night sky (dark blues)
	east: dawn breaking
	west: birds flying towards sunset



possible approach, if we wanted > 1 theme:
AI suggestion for possible usage:


Instead of hardcoding: Create texture set directories with consistent naming

textures/
├── sunset_theme/
│   ├── north_wall.png
│   ├── south_wall.png  
│   ├── east_wall.png
│   └── west_wall.png
├── medieval_theme/
│   ├── north_wall.png
│   ├── south_wall.png
│   ├── east_wall.png
│   └── west_wall.png
└── space_theme/
    ├── north_wall.png
    ├── south_wall.png
    ├── east_wall.png
    └── west_wall.png



Configuration File Approach:

# In your .cub file - specify theme instead of individual files
TEXTURE_SET sunset_theme

# Or maintain current approach for maximum control
NO ./textures/sunset_theme/north_wall.png
SO ./textures/sunset_theme/south_wall.png
WE ./textures/sunset_theme/west_wall.png  
EA ./textures/sunset_theme/east_wall.png