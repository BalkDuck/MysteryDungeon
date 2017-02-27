#ifndef DUNGEON_GENERATOR_H
#define DUNGEON_GENERATOR_H

#include "dungeon\dungeon.h"

int DrawTileToSurface(Floor* floor, int x, int y);

int AddFloorSurface(Floor* floor);

Floor* GenerateFloor(int seed);

Dungeon* GenerateDungeon(int dungeon_name, int seed);

#endif