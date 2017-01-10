#ifndef COLOR_UTIL_H
#define COLOR_UTIL_H

#include <SDL.h>

static int LIMIT(int x, int min, int max) {
	if (x > max) x = max;
	if (x < min) x = min;
	return x;
}

static double ANGLE(double angle) {
	while (angle >= 360)	angle -= 360;
	while (angle < 0)		angle += 360;
	return angle;
}

static int MIN(int a, int b) {
	return (a <= b ? a : b);
}

static int MIN(int a, int b, int c) {
	return MIN(a, MIN(b, c));
}

static int MAX(int a, int b) {
	return (a > b ? a : b);
}

static int MAX(int a, int b, int c) {
	return MAX(a, MAX(b, c));
}

static int MID(int a, int b, int c) {
	if (a == b) return a;
	if (b == c) return b;
	if (c == a) return c;
	int min = MIN(a, b, c);
	int max = MAX(a, b, c);
	if (a > min && a < max) return a;
	if (b > min && b < max) return b;
	return c;
}

struct RGB {
	int r;
	int g;
	int b;
};

static RGB Uint32toRGB(Uint32 in) {
	RGB out;
	out.r = (in >> 16) & 0xFF;
	out.g = (in >> 8) & 0xFF;
	out.b = in & 0xFF;
	return out;
}

static Uint32 RGBtoUint32(RGB in) {
	Uint32 r = (Uint32)in.r;
	Uint32 g = (Uint32)in.g;
	Uint32 b = (Uint32)in.b;
	Uint32 out = (r << 16) + (g << 8) + b;
	return out;
}

#define MOD_R 0
#define MOD_G 1
#define MOD_B 2

/**
 * Modifies a single RGB value, and determines if there is
 * any remainder that needs to be used to modify other values
 */
static RGB* ModRGBValue(RGB* in, int mod, int which_value) {
	if (!mod) return in;

	int min = MIN(in->r, in->g, in->b);
	int max = MAX(in->r, in->g, in->b);

	int prev_value;
	switch (which_value) {
	case MOD_R:
		prev_value = in->r;
		in->r += mod;
		if (in->r >= max) {
			in->r = max;
			mod -= max - prev_value;
			if (in->g == max) {
				return ModRGBValue(in, -mod, MOD_G);
			}
			else {
				return ModRGBValue(in, -mod, MOD_B);
			}
		}
		else if (in->r <= min) {
			in->r = min;
			mod += prev_value - min;
			if (in->g == min) {
				return ModRGBValue(in, -mod, MOD_G);
			}
			else {
				return ModRGBValue(in, -mod, MOD_B);
			}
		}
		break;

	case MOD_G:
		prev_value = in->g;
		in->g += mod;
		if (in->g >= max) {
			in->g = max;
			mod -= max - prev_value;
			if (in->r == max) {
				return ModRGBValue(in, -mod, MOD_R);
			}
			else {
				return ModRGBValue(in, -mod, MOD_B);
			}
		}
		else if (in->g <= min) {
			in->g = min;
			mod += prev_value - min;
			if (in->r == min) {
				return ModRGBValue(in, -mod, MOD_R);
			}
			else {
				return ModRGBValue(in, -mod, MOD_B);
			}
		}
		break;

	case MOD_B:
		prev_value = in->b;
		in->b += mod;
		if (in->b >= max) {
			in->b = max;
			mod -= max - prev_value;
			if (in->r == max) {
				return ModRGBValue(in, -mod, MOD_R);
			}
			else {
				return ModRGBValue(in, -mod, MOD_G);
			}
		}
		else if (in->b <= min) {
			in->b = min;
			mod += prev_value - min;
			if (in->r == min) {
				return ModRGBValue(in, -mod, MOD_R);
			}
			else {
				return ModRGBValue(in, -mod, MOD_G);
			}
		}
		break;
	}

	return in;
}

/**
 * Takes in an RGB pointer and a hue value
 * Hue is treated as an angle: passing multiples of 360 
 * will get the same RGB value in return
 */
static RGB* ModRGBHue(RGB* in, int hue) {
	int min = MIN(in->r, in->g, in->b);
	int max = MAX(in->r, in->g, in->b);
	if (min == max) return in;
	int mod = hue * (max - min) * 6 / 360;
	
	int mid = MID(in->r, in->g, in->b);
	if (mid == max || mid == min) {
		if (in->r == in->g) {
			return ModRGBValue(in, mod, MOD_R);
		}
		else if (in->g == in->b) {
			return ModRGBValue(in, mod, MOD_G);
		}
		else {
			return ModRGBValue(in, mod, MOD_B);
		}
	}

	if (in->r == mid) {
		return ModRGBValue(in, mod, MOD_R);
	}
	else if (in->g == mid) {
		return ModRGBValue(in, mod, MOD_G);
	}
	else {
		return ModRGBValue(in, mod, MOD_B);
	}

	return in;
}

#endif