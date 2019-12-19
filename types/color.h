#ifndef HEZ_COLOR_H
#define HEZ_COLOR_H

namespace hez {
	struct color {
		union {
			struct {
				unsigned char b, g, r, a;
			};
			unsigned int c;
		};
		inline color() { c = 0; }
		static inline color rgba(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) {
			color cl;
			cl.r = _r;
			cl.g = _g;
			cl.b = _b;
			cl.a = _a;
			return cl;
		} 
	};
}

#endif
