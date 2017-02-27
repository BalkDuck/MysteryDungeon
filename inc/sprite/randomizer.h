#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include "color_util.h"

#define BUCKET_SIZE 100

#define WEIGHT 50

struct ColorBucketItem {
	Uint32 color;
	ColorBucketItem* next = nullptr;
};

struct ColorBucket {
	int bucket_size;
	int weight;
	ColorBucketItem* head = nullptr;
	ColorBucketItem* tail = nullptr;
	ColorBucket* next = nullptr;
};

struct ColorBucketList {
	ColorBucket* head = nullptr;
	ColorBucket* tail = nullptr;
};

static int colors_to_skip[] = {
	0xFF00FF, // Magic pink
	0xFFFFFF, // White
	0x000000, // Black
};

Uint32 GetColor(SDL_Surface* surface, int x, int y);

/**
 * Returns a slightly-randomized bitmap
 * THIS WILL MODIFY THE BITMAP PASSED IN, NOT COPY IT
 * WATCH YOUR POINTERS
 */
SDL_Surface* RandomizeSurface(SDL_Surface* surface, int seed, int bucket_size, int weight);

#endif