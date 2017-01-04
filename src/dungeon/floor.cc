#include "dungeon\floor.h"

#define MIN_ROOM_BUFFER 3

#define MAX_PLACEMENT_FAILURES 1000

int start_x = 0;
int start_y = 0;

int GetRandom(int min, int max) {
	int random = rand() % (abs(max - min)) + min;
	return random;
}

/** Difficulty variables */

int difficulty;
int floor_w = 100;
int floor_h = 100;
int min_room_dimension = 5;
int max_room_dimension = 10;
int total_corridor_length;

void SetDifficultyVariables() {
	/**
	TODO: Make these variables dependent on difficulty

	floor_w;
	floor_h;
	min_room_dimension;
	max_room_dimension;
	*/
	total_corridor_length = floor_w * floor_h / 10;
}

/** Tile functions */

int GetTile(Floor floor, int x, int y) {
	if (x < 0 || x >= floor_w || y < 0 || y >= floor_h) {
		return tile_boundary;
	}
	return floor[x][y];
}

int SetTile(Floor floor, int x, int y, int tile) {
	if (x < 0 || x >= floor_w || y < 0 || y >= floor_h) {
		return 0;
	}
	floor[x][y] = tile;
	return 1;
}

/** Corridors */

int AddCorridor(Floor floor, int ax, int ay, int bx, int by) {
	int x_diff = ax > bx ? ax - bx : bx - ax;
	int y_diff = ay > by ? ay - by : by - ay;
	int mid;
	if (x_diff > y_diff) {
		mid = ax < bx ? GetRandom(ax, bx) : GetRandom(bx, ax);
		while (ax != mid) {
			SetTile(floor, ax, ay, tile_hall);
			ax += ax < mid ? +1 : -1;
		}
		while (bx != mid) {
			SetTile(floor, bx, by, tile_hall);
			bx += bx < mid ? +1 : -1;
		}
		while (ay != by) {
			SetTile(floor, ax, ay, tile_hall);
			ay += ay < by ? +1 : -1;
		}
		SetTile(floor, ax, ay, tile_hall);
	}
	else {
		mid = ay < by ? GetRandom(ay, by) : GetRandom(by, ay);
		while (ay != mid) {
			SetTile(floor, ax, ay, tile_hall);
			ay += ay < mid ? +1 : -1;
		}
		while (by != mid) {
			SetTile(floor, bx, by, tile_hall);
			by += by < mid ? +1 : -1;
		}
		while (ax != bx) {
			SetTile(floor, ax, ay, tile_hall);
			ax += ax < bx ? +1 : -1;
		}
		SetTile(floor, ax, ay, tile_hall);
	}
	return 1;
}

int IsTilePassable(Floor floor, int x, int y) {
	switch (GetTile(floor, x, y)) {
	case tile_room:
		return 1;
	case tile_hall:
		return 1;
	case tile_stairs:
		return 1;
	default:
		return 0;
	}
}

int CheckCorner(Floor floor, int x, int y) {
	int tile_n = IsTilePassable(floor, x, y - 1);
	int tile_s = IsTilePassable(floor, x, y + 1);
	int tile_e = IsTilePassable(floor, x + 1, y);
	int tile_w = IsTilePassable(floor, x - 1, y);
	int tile_ne = IsTilePassable(floor, x + 1, y - 1);
	int tile_nw = IsTilePassable(floor, x - 1, y - 1);
	int tile_se = IsTilePassable(floor, x + 1, y + 1);
	int tile_sw = IsTilePassable(floor, x - 1, y + 1);
	if (tile_n && tile_ne && tile_e && (!tile_s || !tile_w)) return 1;
	if (tile_n && tile_nw && tile_w && (!tile_s || !tile_e)) return 1;
	if (tile_s && tile_se && tile_e && (!tile_n || !tile_w)) return 1;
	if (tile_s && tile_sw && tile_w && (!tile_n || !tile_e)) return 1;
	return 0;
}

int RemoveWideCorridors(Floor floor) {
	for (int x = 0; x < floor_w; x++) {
		for (int y = 0; y < floor_h; y++) {
			if (GetTile(floor, x, y) == tile_hall && CheckCorner(floor, x, y)) {
				SetTile(floor, x, y, tile_unused);
			}
		}
	}
	return 1;
}

/** Rooms */

int CheckRoomPlacement(int** floor, int x, int y, int w, int h) {
	if (x < 0 || y < 0 || x + w > floor_w || y + h > floor_h) {
		return 0;
	}

	/** Check against the size of the room plus the minimum boundary */
	int new_x = x - MIN_ROOM_BUFFER >= 0 ? x - MIN_ROOM_BUFFER : 0;
	int new_y = y - MIN_ROOM_BUFFER >= 0 ? y - MIN_ROOM_BUFFER : 0;
	int max_x = x + w + MIN_ROOM_BUFFER < floor_w ? x + w + MIN_ROOM_BUFFER : floor_w;
	int max_y = y + h + MIN_ROOM_BUFFER < floor_h ? y + h + MIN_ROOM_BUFFER : floor_h;

	/** Make sure the new room won't intersect any room tiles and intersects at least one hall tile */
	for (int px = new_x; px < max_x; px++) {
		for (int py = new_y; py < max_y; py++) {
			if (GetTile(floor, px, py) == tile_room) return 0;
		}
	}
	int hall_tile_found = 0;
	for (int px = x; px < x + w; px++) {
		for (int py = y; py < y + h; py++) {
			if (GetTile(floor, px, py) == tile_hall) hall_tile_found = 1;
		}
	}

	/** Placement might be good! */
	return hall_tile_found;
}

int AddRoom(int** floor, int x, int y, int w, int h) {
	if (!CheckRoomPlacement(floor, x, y, w, h)) {
		return 0;
	}

	/** Place room border */
	for (int i = x; i < x + w; i++) {
		int north_tile = GetTile(floor, i, y);
		if (north_tile == tile_unused) SetTile(floor, i, y, tile_wall);
		int south_tile = GetTile(floor, i, y + h - 1);
		if (south_tile == tile_unused) SetTile(floor, i, y + h - 1, tile_wall);
	}
	for (int i = y; i < y + h; i++) {
		int west_tile = GetTile(floor, x, i);
		if (west_tile == tile_unused) SetTile(floor, x, i, tile_wall);
		int east_tile = GetTile(floor, x + w - 1, i);
		if (east_tile == tile_unused) SetTile(floor, x + w - 1, i, tile_wall);
	}

	/** Fill room with passable tiles */
	for (int px = x + 1; px < x + w - 1; px++) {
		for (int py = y + 1; py < y + h - 1; py++) {
			SetTile(floor, px, py, tile_room);
		}
	}
	
	/** Add extra elements */

	return 1;
}

/** Floor generator */

Floor GenerateFloor(int pdifficulty, int pseed) {
	srand(pseed);

	/** Determine dimensions and corridor density */
 	difficulty = pdifficulty;
 	SetDifficultyVariables();

	/** Fill the floor with unused tiles */
	Floor floor = new int*[floor_w];
	for (int x = 0; x < floor_w; x++) {
		floor[x] = new int[floor_h];
		for (int y = 0; y < floor_h; y++) {
			SetTile(floor, x, y, tile_unused);
		}
	}

	/** Place floor boundaries */
	for (int x = 0; x < floor_w; x++) {
		SetTile(floor, x, 0, tile_boundary);
		SetTile(floor, x, floor_h - 1, tile_boundary);
	}
	for (int y = 0; y < floor_h; y++) {
		SetTile(floor, 0, y, tile_boundary);
		SetTile(floor, floor_w - 1, y, tile_boundary);
	}

	/** Fill with corridors */
	int hall_start_x = GetRandom(1, floor_w - 2);
	int hall_start_y = GetRandom(1, floor_h - 2);
	int length = 0;
	while (length < floor_w * floor_h / 10) {
		int dest_x = GetRandom(1, floor_w - 2);
		int dest_y = GetRandom(1, floor_h - 2);
		AddCorridor(floor, hall_start_x, hall_start_y, dest_x, dest_y);
		length += abs(dest_x - hall_start_x) + abs(dest_y - hall_start_y);
		hall_start_x = dest_x;
		hall_start_y = dest_y;
		RemoveWideCorridors(floor);
	}

	/** Place rooms */
	int failure_count = 0;
	int room_count = 0;
	while (failure_count < MAX_PLACEMENT_FAILURES || !room_count) {
		int w = GetRandom(min_room_dimension, floor_w / min_room_dimension);
		int h = GetRandom(min_room_dimension, floor_h / min_room_dimension);
		int x = GetRandom(0, floor_w - w);
		int y = GetRandom(0, floor_h - h);
		if (!AddRoom(floor, x, y, w, h)) {
			failure_count++;
		}
		else {
			if (!room_count) {
				/** Initial room added */
				while (GetTile(floor, start_x, start_y) != tile_room) {
					start_x = GetRandom(x, x + w);
					start_y = GetRandom(y, y + h);
				}
			}
			room_count++;
		}
	}

	/** Place stairs */
	int stairs_placed = 0;
	while (!stairs_placed) {
		int x = GetRandom(1, floor_w - 2);
		int y = GetRandom(1, floor_h - 2);
		if (GetTile(floor, x, y) == tile_room) {
			SetTile(floor, x, y, tile_stairs);
			stairs_placed = true;
		}
	}

	/** Place items and other extras */

	/** Spawn initial wave of Pokemon */

	/** Fill in remaining unused tiles */
	for (int x = 0; x < floor_w; x++) {
		for (int y = 0; y < floor_h; y++) {
			if (GetTile(floor, x, y) == tile_unused) SetTile(floor, x, y, tile_wall);
		}
	}

	return floor;
}

int DeleteFloor(Floor floor) {
	for (int i = 0; i < floor_w; i++) {
		delete[] floor[i];
	}
	delete[] floor;
	return 1;
}

/** Debugging */

void PrintFloor(Floor floor) {
	std::string tile_type_strings[] = {
		"!",
		"=",
		"M",
		".",
		".",
		"X",
	};
	for (int y = 0; y < floor_h; y++) {
		for (int x = 0; x < floor_w; x++) {
			std::cout << tile_type_strings[GetTile(floor, x, y)].c_str();
		}
		std::cout << std::endl;
	}
}