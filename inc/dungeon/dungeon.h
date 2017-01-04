#ifndef DUNGEON_H
#define DUNGEON_H

#include "dungeon\dungeon_template.h"
#include "dungeon\floor.h"
#include <iostream>

struct Dungeon {
	DungeonTemplate dungeon_template;
	int seed;
	int* floor_seeds;
	int floor_count;
	int current_floor;
	Floor floor;
};

Dungeon* GenerateDungeon(int, int);

int GoToNextFloor(Dungeon*);

#endif