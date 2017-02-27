#ifndef TILE_H
#define TILE_H

#define TILE_DIMENSION 24

typedef enum
{
	tile_unused,
	tile_boundary,
	tile_wall,
	tile_room,
	tile_hall,
	tile_stairs,
	tile_lava,
} Tile;

#endif