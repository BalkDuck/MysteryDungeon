#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <map>
#include <SDL.h>
#include <string>
#include <vector>

#include "sprite\color_util.h"

/** Magic pink color key */
#define COLOR_KEY_HEX 0xFF00FF

/** Pointer to vector of colors */
typedef std::vector<Color>* Bucket;

/** Returns the Uint32 value of the pixel at the coordinates */
Uint32 GetPixel(SDL_Surface*, int, int);

/** Replaces the pixel at the coordinates with the passed Uint32 value */
void PutPixel(SDL_Surface*, Uint32, int, int);

void ReplaceColor(SDL_Surface*, Uint32, Uint32);

struct BucketContainer {

	/** Vector of all colors in a bitmap */
	std::vector<Color> colors;

	/** Map to store total occurances of each color in the bitmap */
	std::map<Uint32, int> colorCount;

	/** Map to store which colors have already been bucketed */
	std::map<Uint32, bool> bucketedColors;

	/** Map of uint32 values and their randomized replacements */
	std::map<Uint32, Uint32> modColors;

	/** Vector of color buckets */
	std::vector<Bucket> buckets;

	/** Function to find all colors */
	void FindColors(SDL_Surface*);

	/** Function to build a single bucket */
	Bucket BuildBucket(Bucket, Color, int);

	/** Function to build buckets for all colors */
	void BuildBuckets(int);

	/** Function to randomize all buckets */
	void RandomizeColors(int);

	/** Constructor */
	BucketContainer(SDL_Surface*);

};

/** Main randomizer function */
SDL_Surface* RandomizeBitmap(std::string filepath, int, int);

#endif