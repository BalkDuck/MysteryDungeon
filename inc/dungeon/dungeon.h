#ifndef DUNGEON_H
#define DUNGEON_H

#include "config\config.h"
#include "dungeon\floor.h"

typedef enum
{
	MagmaCavern,
	TinyWoods,
} DungeonName;

static std::string dungeon_name_table[] =
{
	"MagmaCavern",
	"TinyWoods",
};

struct Dungeon
{
	int name;
	int seed;
	int difficulty;
	int* floor_seeds;
	int total_floors;
	int floor_counter;
	Floor* floor;
};

#endif