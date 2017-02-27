#include "config\config.h"

static std::map<char*, SDL_Surface*> bitmap_cache;

SDL_Surface* LoadBMP(char* filepath, int alpha) {
	char* key = filepath + alpha;

	if (!bitmap_cache[key]) {
		SDL_Surface* load_surface = SDL_LoadBMP(filepath);
		SDL_SetColorKey(load_surface, SDL_SRCCOLORKEY, COLOR_KEY_HEX);
		SDL_SetAlpha(load_surface, SDL_SRCALPHA, alpha);
		bitmap_cache[key] = SDL_DisplayFormat(load_surface);
		SDL_FreeSurface(load_surface);
	}

	return bitmap_cache[key];
}

SDL_Surface* LoadBMP(char* filepath, int alpha, int seed) {
	char* key = filepath + alpha + seed;

	if (!bitmap_cache[key]) {
		SDL_Surface* load_surface = RandomizeSurface(SDL_LoadBMP(filepath), seed, BUCKET_SIZE, WEIGHT);
		SDL_SetColorKey(load_surface, SDL_SRCCOLORKEY, COLOR_KEY_HEX);
		SDL_SetAlpha(load_surface, SDL_SRCALPHA, alpha);
		bitmap_cache[key] = SDL_DisplayFormat(load_surface);
		SDL_FreeSurface(load_surface);
	}

	return bitmap_cache[key];
}

SDL_Surface* CreateSurface(int w, int h, Uint32 src_flag) {
	SDL_Surface* to_return = nullptr;
	return to_return;
}