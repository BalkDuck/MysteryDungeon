#include "sprite\randomizer.h"

Uint32 GetPixel(SDL_Surface* bitmap, int x, int y) {
	int bytesPerPixel = bitmap->format->BytesPerPixel;
	Uint8 *pixel = (Uint8 *)bitmap->pixels + y * bitmap->pitch + x * bytesPerPixel;
	switch (bytesPerPixel) {
	case 1:
		return *pixel;
		break;

	case 2:
		return *(Uint16 *)pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
		else
			return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
		break;

	case 4:
		return *(Uint32 *)pixel;
		break;
	}
	return 0;
}

void PutPixel(SDL_Surface* bitmap, int x, int y, Uint32 pixel) {
	int bpp = bitmap->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)bitmap->pixels + y * bitmap->pitch + x * bpp;
	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16 *)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}

void ReplaceColor(SDL_Surface* bitmap, Uint32 color, Uint32 replace) {
	for (int x = 0; x < bitmap->w; x++) {
		for (int y = 0; y < bitmap->h; y++) {

			Uint32 pixel = GetPixel(bitmap, x, y);

			if (pixel == color) {
				PutPixel(bitmap, x, y, replace);
			}

		}
	}
}

void BucketContainer::FindColors(SDL_Surface* bitmap) {
	/** Iterate over all pixels in the bitmap */
	for (int x = 0; x < bitmap->w; x++) {
		for (int y = 0; y < bitmap->h; y++) {

			/** Get the pixel at the coordinates */
			Uint32 pixel = GetPixel(bitmap, x, y);

			/** Check if the color matches the background color */
			if (pixel != COLOR_KEY_HEX) {

				/** Check if the color has already been visited */
				if (colorCount.count(pixel) == 0) {

					/** Mark the color as visited */
					colorCount[pixel] = 1;

					/** Mark the color as not bucketed */
					bucketedColors[pixel] = false;

					/** Push the color to the vector */
					colors.push_back(BuildColor(pixel));

				}
				else {

					/** Increment count of color */
					colorCount[pixel] += 1;

				}
			}
		}
	}
}

/**
 * 
 * Use recursion to find all colors belonging to one bucket.
 * A color belongs to the bucket if the difference between its hue value
 * and the hue value of at least one other color in the bucket is less
 * than the weight.
 * For every new color added, make a recursive call to add colors similar
 * to it.
 * 
 */
Bucket BucketContainer::BuildBucket(Bucket bucket, Color base, int weight) {
	for (Color color : colors) {

		/** Check if the color has already been bucketed */
		if (!bucketedColors[color.uint32]) {

			/** Get the difference in the two colors' hue values */
			double delta = abs(color.hsv.h - base.hsv.h);

			/** If the difference is less than the weight... */
			if (delta < weight || delta > 360 - weight) {

				/** Mark color as bucketed */
				bucketedColors[color.uint32] = true;

				/** Add color to bucket */
				bucket->push_back(color);

				/** Make recursive call to add more colors */
				BuildBucket(bucket, color, weight);

			}
		}
	}

	return bucket;
}

void BucketContainer::BuildBuckets(int weight) {
	for (Color color : colors) {

		/** Check if color has been bucketed */
		if (!bucketedColors[color.uint32]) {

			/** Mark the color as bucketed */
			bucketedColors[color.uint32] = true;

			/** Build a new bucket */
			Bucket bucket = new std::vector<Color>;

			/** Add color to bucket */
			bucket->push_back(color);

			/** Add the bucket to the vector */
			buckets.push_back(BuildBucket(bucket, color, weight));

		}
	}
}

void BucketContainer::RandomizeColors(int weight) {
	for (Bucket bucket : buckets) {

		/** Calculate a random hue modifier that will be applied to the bucket */
		double delta = rand() % weight - weight / 2;

		for (Color color : *bucket) {
			HSV mod = {
				color.hsv.h,
				color.hsv.s,
				color.hsv.v,
			};
			mod.h += delta;
			while (mod.h >= 360) 
				mod.h -= 360;
			while (mod.h < 0) 
				mod.h += 360;
			if (mod.s != 0) 
				mod.s -= delta / 255;
			if (mod.s < 0) 
				mod.s = 0;
			if (mod.s > 1) 
				mod.s = 1;
			if (mod.s == 0) 
				mod.v -= delta / 100;
			/** Convert to uint32 and add to the map */
			modColors[color.uint32] = HSVtoUint32(mod);
		}
	}
}

BucketContainer::BucketContainer(SDL_Surface* bitmap) {

	FindColors(bitmap);

}

SDL_Surface* RandomizeBitmap(std::string filepath, int weight, int seed) {
	/** Seed */
	srand(seed);

	/** Load the bitmap */
	SDL_Surface* bitmap = SDL_LoadBMP(filepath.c_str());
	if (bitmap == NULL) {
		return NULL;
	}

	/** Create the bucket container */
	BucketContainer bucketContainer = BucketContainer(bitmap);

	/** Build color buckets based on weight */
	bucketContainer.BuildBuckets(weight);

	/** Randomize colors */
	bucketContainer.RandomizeColors(weight);

	for (Color color : bucketContainer.colors) {
		ReplaceColor(bitmap, color.uint32, bucketContainer.modColors[color.uint32]);
	}

	return bitmap;
}