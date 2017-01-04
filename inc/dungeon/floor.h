#ifndef FLOOR_H
#define FLOOR_H

#include <iostream>

typedef enum {
	tile_unused,
	tile_boundary,
	tile_wall,
	tile_room,
	tile_hall,
	tile_stairs,
} Tile;

typedef int** Floor;

Floor GenerateFloor(int, int);

int DeleteFloor(Floor);

/** Debugging */

void PrintFloor(Floor);

#endif