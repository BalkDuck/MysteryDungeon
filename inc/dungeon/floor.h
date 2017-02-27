#ifndef FLOOR_H
#define FLOOR_H

#include "config\config.h"
#include "dungeon\tile.h"

struct Floor
{
	int** tiles;
	int w;
	int h;
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	SDL_Surface* surface;
};

static int GetTile(Floor* floor, int x, int y)
{
	if (x < 0 || x >= floor->w || y < 0 || y >= floor->h)
	{
		return tile_boundary;
	}
	return floor->tiles[x][y];
}

/**
 * Returns tile_wall if the tile is a boundary. Used to equate boundaries and walls
 * for graphics purposes
 */
static int GetTileBoundaryAsWall(Floor* floor, int x, int y)
{
	if (x < 0 || x >= floor->w || y < 0 || y >= floor->h)
	{
		return tile_wall;
	}
	if (floor->tiles[x][y] == tile_boundary)
	{
		return tile_wall;
	}
	return floor->tiles[x][y];
}

static int SetTile(Floor* floor, int x, int y, int tile)
{
	if (x < 0 || x >= floor->w || y < 0 || y >= floor->h)
	{
		return 0;
	}
	floor->tiles[x][y] = tile;
	return 1;
}

static void PrintFloor(Floor* floor)
{
	std::string strings[] = 
	{
		" ",
		"=",
		"=",
		".",
		".",
		"X",
		"n",
	};
	for (int y = 0; y < floor->h; y++)
	{
		for (int x = 0; x < floor->w; x++)
		{
			std::cout << strings[GetTile(floor, x, y)].c_str();
		}
		std::cout << std::endl;
	}
}

#endif