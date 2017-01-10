#include "sprite\randomizer.h"

static ColorBucketList* buckets = new ColorBucketList();

Uint32 GetColor(SDL_Surface* surface, int x, int y) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	switch (bpp) {
	case 1:
		return *pixel;
	case 2:
		return *(Uint16 *)pixel;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
		}
		else {
			return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
		}
	case 4:
		return *(Uint32 *)pixel;
	}
	return 0x0;
}

void SetColor(SDL_Surface* surface, int x, int y, Uint32 in) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	switch (bpp) {
	case 1:
		*pixel = in;
		break;
	case 2:
		*(Uint16 *)pixel = in;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			pixel[0] = (in >> 16) & 0xff;
			pixel[1] = (in >> 8) & 0xff;
			pixel[2] = in & 0xff;
		} else {
			pixel[0] = in & 0xff;
			pixel[1] = (in >> 8) & 0xff;
			pixel[2] = (in >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *)pixel = in;
		break;
	}
}

void ReplaceColor(SDL_Surface* surface, Uint32 out, Uint32 in) {
	for (int x = 0; x < surface->w; x++) {
		for (int y = 0; y < surface->h; y++) {
			Uint32 color = GetColor(surface, x, y);
			if (color == out) {
				SetColor(surface, x, y, in);
			}
		}
	}
}

int IsSkippedColor(Uint32 color) {
	for (int i = 0; i < sizeof(colors_to_skip) / sizeof(Uint32); i++) {
		if (color == colors_to_skip[i]) {
			return 1;
		}
	}
	return 0;
}

int IsInBucket(Uint32 color) {
	if (IsSkippedColor(color)) {
		return 1;
	}
	ColorBucket* bucket = buckets->head;
	while (bucket != nullptr) {
		ColorBucketItem* item = bucket->head;
		while (item != nullptr) {
			if (item->color == color) {
				return 1;
			}
			item = item->next;
		}
		bucket = bucket->next;
	}
	return 0;
}

int IsWithinBucketSize(Uint32 bucketed, Uint32 color, int bucket_size) {
	RGB bucketed_rgb = Uint32toRGB(bucketed);
	RGB color_rgb    = Uint32toRGB(color);
	int total = (bucketed_rgb.r > color_rgb.r ? bucketed_rgb.r - color_rgb.r : color_rgb.r - bucketed_rgb.r) +
			(bucketed_rgb.g > color_rgb.g ? bucketed_rgb.g - color_rgb.g : color_rgb.g - bucketed_rgb.g) +
			(bucketed_rgb.b > color_rgb.b ? bucketed_rgb.b - color_rgb.b : color_rgb.b - bucketed_rgb.b);
	return total <= bucket_size;
}

void FillBucket(SDL_Surface* surface, ColorBucket* bucket) {
	for (int x = 0; x < surface->w; x++) {
		for (int y = 0; y < surface->h; y++) {
			Uint32 color = GetColor(surface, x, y);

			if (!IsInBucket(color)) {
				ColorBucketItem* item = bucket->head;
				int added = 0;
				while (item != nullptr && !added) {
					if (IsWithinBucketSize(item->color, color, bucket->bucket_size)) {
						/** Create new bucket item */
						ColorBucketItem* new_item = new ColorBucketItem();
						new_item->color = color;
						bucket->tail->next = new_item;
						bucket->tail = new_item;
						added = 1;
					}
					item = item->next;
				}
			}

		}
	}
}

SDL_Surface* RandomizeSurface(SDL_Surface* surface, int seed, int bucket_size, int weight) {
	std::cout << "Seed: " << seed << std::endl;
	srand(seed);
	
	/** Create buckets */
	for (int x = 0; x < surface->w; x++) {
		for (int y = 0; y < surface->h; y++) {
			Uint32 color = GetColor(surface, x, y);
			if (!IsInBucket(color)) {
				ColorBucket* bucket = new ColorBucket();
				ColorBucketItem* item = new ColorBucketItem();
				item->color = color;
				bucket->head = item;
				bucket->tail = item;
				bucket->bucket_size = bucket_size;
				bucket->weight = rand() % weight - weight;
				std::cout << bucket->weight << std::endl;
				FillBucket(surface, bucket);
				if (buckets->tail == nullptr) {
					buckets->head = bucket;
					buckets->tail = bucket;
				}
				else {
					buckets->tail->next = bucket;
					buckets->tail = bucket;
				}
			}
		}
	}

	/** Replace colors */
	ColorBucket* bucket = buckets->head;
	while (bucket != nullptr) {
		ColorBucketItem* item = bucket->head;
		while (item != nullptr) {
			RGB* replacement_rgb = ModRGBHue(&Uint32toRGB(item->color), bucket->weight);
			ReplaceColor(surface, item->color, RGBtoUint32(*replacement_rgb));
			item = item->next;
		}
		bucket = bucket->next;
	}

	/** Delete buckets */
	ColorBucket* bucket_to_delete = buckets->head;
	while (bucket_to_delete != nullptr) {
		ColorBucketItem* item = bucket_to_delete->head;
		while (item != nullptr) {
			ColorBucketItem* next_item = item->next;
			delete item;
			item = next_item;
		}
		ColorBucket* next_bucket = bucket_to_delete->next;
		delete bucket_to_delete;
		bucket_to_delete = next_bucket;
	}
	buckets->head = nullptr;
	buckets->tail = nullptr;

	return surface;
}