#ifndef COLOR_H
#define COLOR_H

#include <SDL.h>

struct RGB {
	double r;
	double g;
	double b;
	/** cout override
	friend std::ostream& operator<<(std::ostream& os, RGB rgb) {
		os << rgb.r << ", " << rgb.g << ", " << rgb.b;
		return os;
	}
	*/
};

struct HSV {
	double h;
	double s;
	double v;
	/** cout override
	friend std::ostream& operator<<(std::ostream& os, HSV hsv) {
		os << hsv.h << ", " << hsv.s << ", " << hsv.v;
		return os;
	}
	*/
};

struct Color {
	RGB rgb;
	HSV hsv;
	Uint32 uint32;
	bool operator==(Color rhs) {
		return (rgb.r == rhs.rgb.r &&
				rgb.g == rhs.rgb.g &&
				rgb.b == rhs.rgb.b);
	}
};

#endif