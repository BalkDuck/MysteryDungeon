/**
MIT License

Copyright (c) 2017 BalkDuck

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "dungeon\dungeon.h"
#include "sprite\randomizer.h"

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

#define TEST_SEED 5

#define TEST_BUCKET_SIZE 100

#define TEST_WEIGHT 50

#define TEST_RANDOMIZATIONS 10

int main(int argc, char** argv) {
	// Dungeon* dungeon = GenerateDungeon(TinyWoods, TEST_SEED);
	// PrintFloor(dungeon->floor);
	// GoToNextFloor(dungeon);
	// PrintFloor(dungeon->floor);

	SDL_Surface* bitmap = SDL_LoadBMP("Pikachu.bmp");
	SDL_Rect dst_rect = {0, 0, 0, 0};

	SDL_Surface* to_save = SDL_CreateRGBSurface(0, bitmap->w * TEST_RANDOMIZATIONS, bitmap->h, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(bitmap, NULL, to_save, &dst_rect);
	srand(TEST_SEED);
	for (int i = 0; i < TEST_RANDOMIZATIONS; i++) {
		dst_rect.x += bitmap->w;
		SDL_Surface* random = SDL_LoadBMP("Pikachu.bmp");
		RandomizeSurface(random, rand(), TEST_BUCKET_SIZE, TEST_WEIGHT);
		SDL_BlitSurface(random, NULL, to_save, &dst_rect);
	}

	SDL_SaveBMP(to_save, "Randomize.bmp");

	return 0;
}