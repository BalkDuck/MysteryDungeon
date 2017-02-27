#include "dungeon\dungeon_generator.h"

#define MIN_ROOM_BUFFER 7
#define MAX_PLACEMENT_FAILURES 100

// Difficulty variables

static int difficulty;
static int floor_w;
static int floor_h;
static int min_room_dimension;
static int max_room_dimension;
static int total_corridor_length;

// Corridors

int AddCorridor(Floor* floor, int ax, int ay, int bx, int by) 
{
	if (ax == bx && ay == by)
	{
		return 0;
	}
	int x_diff = ax > bx ? ax - bx : bx - ax;
	int y_diff = ay > by ? ay - by : by - ay;
	int mid;
	if (x_diff > y_diff) 
	{
		mid = ax < bx ? GetRandom(ax, bx) : GetRandom(bx, ax);
		while (ax != mid) 
		{
			SetTile(floor, ax, ay, tile_hall);
			ax += ax < mid ? +1 : -1;
		}
		while (bx != mid) 
		{
			SetTile(floor, bx, by, tile_hall);
			bx += bx < mid ? +1 : -1;
		}
		while (ay != by) 
		{
			SetTile(floor, ax, ay, tile_hall);
			ay += ay < by ? +1 : -1;
		}
		SetTile(floor, ax, ay, tile_hall);
	}
	else 
	{
		mid = ay < by ? GetRandom(ay, by) : GetRandom(by, ay);
		while (ay != mid) 
		{
			SetTile(floor, ax, ay, tile_hall);
			ay += ay < mid ? +1 : -1;
		}
		while (by != mid) 
		{
			SetTile(floor, bx, by, tile_hall);
			by += by < mid ? +1 : -1;
		}
		while (ax != bx) 
		{
			SetTile(floor, ax, ay, tile_hall);
			ax += ax < bx ? +1 : -1;
		}
		SetTile(floor, ax, ay, tile_hall);
	}
	return 1;
}

int IsTilePassable(Floor* floor, int x, int y)
{
	if (x < 0 || x >= floor_w || y < 0 || y >= floor_h)
	{
		return 0;
	}
	switch (GetTile(floor, x, y)) 
	{
	case tile_room:
		return 1;
	case tile_hall:
		return 1;
	case tile_stairs:
		return 1;
	case tile_lava:
		return 1;
	default:
		return 0;
	}
}

int CheckCorner(Floor* floor, int x, int y)
{
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

int RemoveWideCorridors(Floor* floor)
{
	for (int x = 0; x < floor_w; x++) 
	{
		for (int y = 0; y < floor_h; y++) 
		{
			if (GetTile(floor, x, y) == tile_hall && CheckCorner(floor, x, y)) 
			{
				SetTile(floor, x, y, tile_unused);
			}
		}
	}
	return 1;
}

// Rooms

int CheckRoomPlacement(Floor* floor, int x, int y, int w, int h)
{
	if (x < 0 || y < 0 || x + w > floor_w || y + h > floor_h) 
	{
		return 0;
	}

	// Check against the size of the room plus the minimum boundary
	int new_x = x - MIN_ROOM_BUFFER >= 0 ? x - MIN_ROOM_BUFFER : 0;
	int new_y = y - MIN_ROOM_BUFFER >= 0 ? y - MIN_ROOM_BUFFER : 0;
	int max_x = x + w + MIN_ROOM_BUFFER < floor_w ? x + w + MIN_ROOM_BUFFER : floor_w;
	int max_y = y + h + MIN_ROOM_BUFFER < floor_h ? y + h + MIN_ROOM_BUFFER : floor_h;

	// Make sure the new room won't intersect any room tiles and intersects at least one hall tile
	for (int px = new_x; px < max_x; px++) 
	{
		for (int py = new_y; py < max_y; py++) 
		{
			if (GetTile(floor, px, py) == tile_room) return 0;
		}
	}
	int hall_tile_found = 0;
	for (int px = x; px < x + w; px++) 
	{
		for (int py = y; py < y + h; py++) 
		{
			if (GetTile(floor, px, py) == tile_hall) hall_tile_found = 1;
		}
	}

	// Placement might be good!
	return hall_tile_found;
}

int AddRoom(Floor* floor, int x, int y, int w, int h)
{
	if (!CheckRoomPlacement(floor, x, y, w, h)) 
	{
		return 0;
	}

	// Fill room with passable tiles
	for (int px = x + 1; px < x + w - 1; px++) 
	{
		for (int py = y + 1; py < y + h - 1; py++) 
		{
			SetTile(floor, px, py, tile_room);
		}
	}
	
	// Add extra elements

	return 1;
}

// Terrain

int AddTerrain(Floor* floor, int tile_terrain, int ax, int ay, int bx, int by) 
{
	if (ax == bx && ay == by)
	{
		return 0;
	}
	int x_diff = ax > bx ? ax - bx : bx - ax;
	int y_diff = ay > by ? ay - by : by - ay;
	int mid;
	if (x_diff > y_diff) 
	{
		mid = ax < bx ? GetRandom(ax, bx) : GetRandom(bx, ax);
		while (ax != mid) 
		{
			if (GetTile(floor, ax, ay) == tile_unused)
			{
				SetTile(floor, ax, ay, tile_terrain);
			}
			ax += ax < mid ? +1 : -1;
		}
		while (bx != mid) 
		{
			if (GetTile(floor, bx, by) == tile_unused)
			{
				SetTile(floor, bx, by, tile_terrain);
			}
			bx += bx < mid ? +1 : -1;
		}
		while (ay != by) 
		{
			if (GetTile(floor, ax, ay) == tile_unused)
			{
				SetTile(floor, ax, ay, tile_terrain);
			}
			ay += ay < by ? +1 : -1;
		}
		if (GetTile(floor, ax, ay) == tile_unused)
		{
			SetTile(floor, ax, ay, tile_terrain);
		}
	}
	else 
	{
		mid = ay < by ? GetRandom(ay, by) : GetRandom(by, ay);
		while (ay != mid) 
		{
			if (GetTile(floor, ax, ay) == tile_unused)
			{
				SetTile(floor, ax, ay, tile_terrain);
			}
			ay += ay < mid ? +1 : -1;
		}
		while (by != mid) 
		{
			if (GetTile(floor, bx, by) == tile_unused)
			{
				SetTile(floor, bx, by, tile_terrain);
			}
			by += by < mid ? +1 : -1;
		}
		while (ax != bx) 
		{
			if (GetTile(floor, ax, ay) == tile_unused)
			{
				SetTile(floor, ax, ay, tile_terrain);
			}
			ax += ax < bx ? +1 : -1;
		}
		if (GetTile(floor, ax, ay) == tile_unused)
		{
			SetTile(floor, ax, ay, tile_terrain);
		}
	}
	return 1;
}

// Graphics

SDL_Surface* tilesheet = nullptr;
SDL_Surface* tile_refs = nullptr;

int DrawTileToSurface(Floor* floor, int x, int y)
{
	int tile = GetTileBoundaryAsWall(floor, x, y);
	int tile_n = GetTileBoundaryAsWall(floor, x, y - 1) == tile;
	int tile_s = GetTileBoundaryAsWall(floor, x, y + 1) == tile;
	int tile_e = GetTileBoundaryAsWall(floor, x + 1, y) == tile;
	int tile_w = GetTileBoundaryAsWall(floor, x - 1, y) == tile;
	int tile_ne = GetTileBoundaryAsWall(floor, x + 1, y - 1) == tile;
	int tile_nw = GetTileBoundaryAsWall(floor, x - 1, y - 1) == tile;
	int tile_se = GetTileBoundaryAsWall(floor, x + 1, y + 1) == tile;
	int tile_sw = GetTileBoundaryAsWall(floor, x - 1, y + 1) == tile;
	int tx = 0; int ty = 0;
	int found = 0;
	for (ty = 1; ty < tile_refs->h && !found; ty += 3)
	{
		for (tx = 1; tx < tile_refs->w && !found; tx += 3)
		{
			int pixel_n = GetColor(tile_refs, tx, ty - 1);
			pixel_n == COLOR_KEY_HEX ? pixel_n = 0 : (pixel_n == 0x0 ? pixel_n = 1 : pixel_n = -1);
			int pixel_s = GetColor(tile_refs, tx, ty + 1);
			pixel_s == COLOR_KEY_HEX ? pixel_s = 0 : (pixel_s == 0x0 ? pixel_s = 1 : pixel_s = -1);
			int pixel_e = GetColor(tile_refs, tx + 1, ty);
			pixel_e == COLOR_KEY_HEX ? pixel_e = 0 : (pixel_e == 0x0 ? pixel_e = 1 : pixel_e = -1);
			int pixel_w = GetColor(tile_refs, tx - 1, ty);
			pixel_w == COLOR_KEY_HEX ? pixel_w = 0 : (pixel_w == 0x0 ? pixel_w = 1 : pixel_w = -1);
			int pixel_ne = GetColor(tile_refs, tx + 1, ty - 1);
			pixel_ne == COLOR_KEY_HEX ? pixel_ne = 0 : (pixel_ne == 0x0 ? pixel_ne = 1 : pixel_ne = -1);
			int pixel_nw = GetColor(tile_refs, tx - 1, ty - 1);
			pixel_nw == COLOR_KEY_HEX ? pixel_nw = 0 : (pixel_nw == 0x0 ? pixel_nw = 1 : pixel_nw = -1);
			int pixel_se = GetColor(tile_refs, tx + 1, ty + 1);
			pixel_se == COLOR_KEY_HEX ? pixel_se = 0 : (pixel_se == 0x0 ? pixel_se = 1 : pixel_se = -1);
			int pixel_sw = GetColor(tile_refs, tx - 1, ty + 1);
			pixel_sw == COLOR_KEY_HEX ? pixel_sw = 0 : (pixel_sw == 0x0 ? pixel_sw = 1 : pixel_sw = -1);
			found = !found &&
					(tile_n == pixel_n || pixel_n == -1) &&
					(tile_s == pixel_s || pixel_s == -1) &&
					(tile_e == pixel_e || pixel_e == -1) &&
					(tile_w == pixel_w || pixel_w == -1) &&
					(tile_ne == pixel_ne || pixel_ne == -1) &&
					(tile_nw == pixel_nw || pixel_nw == -1) &&
					(tile_se == pixel_se || pixel_se == -1) &&
					(tile_sw == pixel_sw || pixel_sw == -1);
		}
	}
	tx = (tx - 1) / 3 - 1;
	ty = (ty - 1) / 3 - 1;
	SDL_Rect dst_rect = {x * TILE_DIMENSION, y * TILE_DIMENSION, TILE_DIMENSION, TILE_DIMENSION};
	SDL_Rect src_rect = {tx * TILE_DIMENSION, ty * TILE_DIMENSION, TILE_DIMENSION, TILE_DIMENSION};
	if (tile == tile_room || tile == tile_hall)
	{
		src_rect.x += TILE_DIMENSION * 15;
	}
	if (tile == tile_lava)
	{
		src_rect.x += TILE_DIMENSION * 24;
	}
	if (tile == tile_stairs)
	{
		src_rect.x = TILE_DIMENSION * 27;
		src_rect.y = 0;
	}
	SDL_BlitSurface(tilesheet, &src_rect, floor->surface, &dst_rect);
	return 1;
}

int AddFloorSurface(Floor* floor)
{
	if (tile_refs == nullptr)
	{
		tile_refs = SDL_LoadBMP("tile_refs.bmp");
	}
	SDL_Surface* surface = SDL_CreateRGBSurface(0, floor->w * TILE_DIMENSION, floor->h * TILE_DIMENSION, 32, rmask, gmask, bmask, amask);
	floor->surface = surface;
	if (tilesheet == NULL)
	{
		std::cout << "Null" << std::endl;
	}
	for (int y = 0; y < floor->h; y++)
	{
		for (int x = 0; x < floor->w; x++)
		{
			DrawTileToSurface(floor, x, y);
		}
	}
	return 1;
}

// Generation functions

Floor* GenerateFloor(int seed)
{
	Floor* floor = new Floor();
	floor->w = floor_w;
	floor->h = floor_h;
	srand(seed);
	
	// Fill floor with unused tiles
	floor->tiles = new int*[floor_w];
	for (int x = 0; x < floor_w; x++)
	{
		floor->tiles[x] = new int[floor_h];
		for (int y = 0; y < floor_h; y++)
		{
			SetTile(floor, x, y, tile_unused);
		}
	}
	
	// Place floor boundaries
	for (int x = 0; x < floor->w; x++)
	{
		SetTile(floor, x, 0, tile_boundary);
		SetTile(floor, x, floor->h - 1, tile_boundary);
	}
	for (int y = 0; y < floor->h; y++)
	{
		SetTile(floor, 0, y, tile_boundary);
		SetTile(floor, floor->w - 1, y, tile_boundary);
	}
	
	// Fill with corridors
	int hall_start_x = GetRandom(1, floor_w - 2);
	int hall_start_y = GetRandom(1, floor_h - 2);
	int length = 0;
	while (length < total_corridor_length) 
	{
		int dest_x = GetRandom(1, floor_w - 2);
		int dest_y = GetRandom(1, floor_h - 2);
		AddCorridor(floor, hall_start_x, hall_start_y, dest_x, dest_y);
		length += abs(dest_x - hall_start_x) + abs(dest_y - hall_start_y);
		hall_start_x = dest_x;
		hall_start_y = dest_y;
		RemoveWideCorridors(floor);
	}
	
	// Place rooms
	int failure_count = 0;
	int room_count = 0;
	while (failure_count < MAX_PLACEMENT_FAILURES || !room_count)
	{
		int w = GetRandom(min_room_dimension, floor_w / min_room_dimension);
		int h = GetRandom(min_room_dimension, floor_h / min_room_dimension);
		int x = GetRandom(0, floor_w - w);
		int y = GetRandom(0, floor_h - h);
		if (!AddRoom(floor, x, y, w, h)) 
		{
			failure_count++;
		}
		else 
		{
			if (!room_count) 
			{
				// Initial room added
				while (GetTile(floor, floor->start_x, floor->start_y) != tile_room) 
				{
					floor->start_x = GetRandom(x, x + w);
					floor->start_y = GetRandom(y, y + h);
				}
			}
			room_count++;
		}
	}

	// Place terrain
	for (int i = 0; i < floor->w / 10; i++)
	{
		int terrain_start_x = GetRandom(1, floor_w - 2);
		int terrain_start_y = GetRandom(1, floor_h - 2);
		int terrain_length = 0;
		while (terrain_length < total_corridor_length * 5) 
		{
			int dest_x = GetRandom(MAX(1, terrain_start_x - 4), MIN(floor_w - 2, terrain_start_x + 4));
			int dest_y = GetRandom(MAX(1, terrain_start_y - 4), MIN(floor_w - 2, terrain_start_y + 4));
			AddTerrain(floor, tile_lava, terrain_start_x, terrain_start_y, dest_x, dest_y);
			terrain_length += abs(dest_x - terrain_start_x) + abs(dest_y - terrain_start_y);
			terrain_start_x = dest_x;
			terrain_start_y = dest_y;
		}
	}
	
	// Place stairs
	int stairs_placed = 0;
	while (!stairs_placed) 
	{
		int x = GetRandom(1, floor_w - 2);
		int y = GetRandom(1, floor_h - 2);
		if (GetTile(floor, x, y) == tile_room
				&& GetTile(floor, x + 1, y) != tile_hall
				&& GetTile(floor, x - 1, y) != tile_hall
				&& GetTile(floor, x, y + 1) != tile_hall
				&& GetTile(floor, x, y - 1) != tile_hall)
		{
			SetTile(floor, x, y, tile_stairs);
			stairs_placed = true;
		}
	}

	// Fill unused tiles
	for (int x = 0; x < floor->w; x++)
	{
		for (int y = 0; y < floor->h; y++)
		{
			if (GetTile(floor, x, y) == tile_unused)
			{
				SetTile(floor, x, y, tile_wall);
			}
		}
	}
	
	// Add graphics
	AddFloorSurface(floor);

	return floor;
}

Dungeon* GenerateDungeon(int dungeon_name, int seed)
{
	Dungeon* dungeon = new Dungeon();
	dungeon->name = dungeon_name;
	dungeon->seed = seed;
	srand(seed);

	// Calculate difficulty variables
	dungeon->difficulty = 5;
	floor_w = 40;
	floor_h = 40;
	min_room_dimension = 7;
	max_room_dimension = 12;
	total_corridor_length = floor_w * floor_h / 10;

	dungeon->total_floors = 10;
	dungeon->floor_counter = 0;

	// Generate floor seeds in advance
	dungeon->floor_seeds = new int[100];
	for (int i = 0; i < dungeon->total_floors; i++)
	{
		dungeon->floor_seeds[i] = rand();
	}

	// Load graphics
	tilesheet = SDL_LoadBMP((dungeon_name_table[dungeon->name] + ".bmp").c_str());

	// Generate the first floor
	dungeon->floor = GenerateFloor(dungeon->floor_seeds[0]);

	return dungeon;
}