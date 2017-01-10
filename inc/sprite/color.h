#ifndef COLOR_H
#define COLOR_H

#include <SDL.h>

typedef Uint32 Pixel;

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
	bool operator==(HSV rhs) {
		return (h == rhs.h && s == rhs.s && v == rhs.v);
	}
	bool operator!=(HSV rhs) {
		return (h != rhs.h || s != rhs.s || v != rhs.v);
	}
	/** cout override
	friend std::ostream& operator<<(std::ostream& os, HSV hsv) {
		os << hsv.h << ", " << hsv.s << ", " << hsv.v;
		return os;
	}
	*/
};

#endif