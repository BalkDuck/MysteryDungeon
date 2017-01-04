#include "dungeon\dungeon.h"

Dungeon* GenerateDungeon(int dungeon_name, int seed) {
	Dungeon* dungeon = new Dungeon();
	DungeonTemplate dungeon_template = dungeon_template_table[dungeon_name];
	dungeon->dungeon_template = dungeon_template;

	/** Store and apply the dungeon's seed */
	dungeon->seed = seed;
	srand(seed);

	/** Generate seeds for all floors */
	dungeon->floor_count = dungeon_template.floor_count;
	dungeon->floor_seeds = new int[dungeon->floor_count];
	for (int i = 0; i < dungeon->floor_count; i++) {
		dungeon->floor_seeds[i] = rand();
	}

	/** Generate the first floor */
	dungeon->floor = GenerateFloor(dungeon_template.difficulty, dungeon->floor_seeds[0]);
	dungeon->current_floor = 0;

	return dungeon;
}

int GoToNextFloor(Dungeon* dungeon) {
	dungeon->current_floor += 1;
	if (dungeon->current_floor == dungeon->floor_count) {
		return 1;
	}
	DeleteFloor(dungeon->floor);
	dungeon->floor = GenerateFloor(dungeon->dungeon_template.difficulty, dungeon->floor_seeds[dungeon->current_floor]);
	return 0;
}