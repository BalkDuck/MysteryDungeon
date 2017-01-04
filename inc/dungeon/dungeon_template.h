#ifndef DUNGEON_TEMPLATE_H
#define DUNGEON_TEMPLATE_H

#include <string>

/** Dungeon names */

typedef enum {
	TinyWoods,
} DungeonName;

static std::string dungeon_name_strings[] = {
	"Tiny Woods",
};

/** Encounterable Pokemon */

#define MAX_RANDOM_ENCOUNTERS 100

struct RandomEncounter {
	int min_floor;
	int max_floor;
	int min_level;
	int max_level;
};

#define MAX_FORCED_ENCOUNTERS 10

struct ForcedEncounter {
	int floor;
	int x;
	int y;
};

/** Dungeon template and table */

static struct DungeonTemplate {
	DungeonName dungeon_name;
	int difficulty;
	int floor_count;
	RandomEncounter random_encounters[MAX_RANDOM_ENCOUNTERS];
	ForcedEncounter forced_encounters[MAX_FORCED_ENCOUNTERS];
	
} dungeon_template_table[] = {
	{
		/** Dungeon name:	*/	TinyWoods,
		/** Difficulty:		*/	1,
		/** Floor count:	*/	3,
		/** Random encounters:	*/
		{
			{
				/** Min floor: */ 1,
				/** Max floor: */ 3,
				/** Min level: */ 2,
				/** Max level: */ 4,
			}
		},
		/** Forced encounters:	 */
		{

		},
	}
};

#endif