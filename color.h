#ifndef COLOR_H
#define COLOR_H

namespace hez {
	struct color {
		union {
			struct {
				unsigned char b, g, r, a;
			};
			unsigned int c;
		};
		inline color() { c = 0; }
		inline color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) : r(_r), g(_g), b(_b), a(_a) { }
	}; 
}

#endif
