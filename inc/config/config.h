#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>

#include <map>
#include <vector>

#include <SDL.h>

#define COLOR_KEY_HEX 0xFF00FF

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	#define rmask 0xff000000
	#define gmask 0x00ff0000
	#define bmask 0x0000ff00
	#define amask 0x000000ff
#else
	#define rmask 0x000000ff
	#define gmask 0x0000ff00
	#define bmask 0x00ff0000
	#define amask 0xff000000
#endif

#include "sprite\randomizer.h"

SDL_Surface* LoadBMP(char* filepath, int alpha);

SDL_Surface* LoadBMP(char* filepath, int alpha, int seed);

SDL_Surface* CreateSurface(int w, int h, Uint32 src_flag);

#define Screen SDL_Surface*

struct ConfigObject {
	Screen screen;
};

#define Config ConfigObject*

Config GetConfig();

static int GetRandom(int min, int max) {
	int random = rand() % (abs(max - min)) + min;
	return random;
}

#endif