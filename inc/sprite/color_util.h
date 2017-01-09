#ifndef COLOR_UTIL_H
#define COLOR_UTIL_H

#include "color.h"

static RGB Uint32toRGB(Uint32 in) {
	RGB out;
	out.r = (in >> 16) & 0xff;
	out.g = (in >> 8) & 0xff;
	out.b = in & 0xff;
	return out;
}

static RGB HSVtoRGB(HSV in) {
	RGB out;

	if (in.s == 0 && in.v == 0) {
		out.r = 0; out.g = 0; out.b = 0;
		return out;
	}

	double hh = in.h / 60;
	long i = (long)hh;
	double ff = hh - i;
	double p = in.v * (1 - in.s);
	double q = in.v * (1 - in.s * ff);
	double t = in.v * (1 - in.s * (1 - ff));

	switch (i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}

	return out;
}

static HSV RGBtoHSV(RGB in) {
	HSV out;

	double min;
	min = in.r < in.g ? in.r : in.g;
	min = min  < in.b ? min : in.b;

	double max;
	max = in.r > in.g ? in.r : in.g;
	max = max  > in.b ? max : in.b;
	out.v = max;

	double d = max - min;

	if (d < 0) {
		out.h = 0; out.s = 0; out.v = 0;
		return out;
	}

	if (max == 0) {
		out.h = 0; out.s = 0; out.v = 0;
		return out;
	}

	out.s = d / max;

	if (d > 0) {
		if (max == in.r) {
			out.h = (in.g - in.b) / d;
		}
		if (max == in.g) {
			out.h = 2 + (in.b - in.r) / d;
		}
		if (max == in.b) {
			out.h = 4 + (in.r - in.g) / d;
		}
	}
	else {
		out.h = 0;
	}

	out.h *= 60;

	while (out.h >= 360) {
		out.h -= 360;
	}
	while (out.h < 0) {
		out.h += 360;
	}

	return out;
}

static HSV Uint32toHSV(Uint32 in) {
	RGB rgb = Uint32toRGB(in);
	return RGBtoHSV(rgb);
}

static Uint32 RGBtoUint32(RGB in) {
	Uint32 r = (Uint32)in.r;
	Uint32 g = (Uint32)in.g;
	Uint32 b = (Uint32)in.b;
	Uint32 out = (r << 16) + (g << 8) + b;
	return out;
}

static Uint32 HSVtoUint32(HSV in) {
	RGB rgb = HSVtoRGB(in);
	return RGBtoUint32(rgb);
}

static Color BuildColor(Uint32 in) {
	/** Create a Color object from a Uint32 value taken from a bitmap */
	Color out = {
		Uint32toRGB(in),
		Uint32toHSV(in),
		in,
	};
	return out;
}

#endif