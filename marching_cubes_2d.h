#ifndef HEZ_MARCHING_CUBES_2D
#define HEZ_MARCHING_CUBES_2D

#include "color.h"

namespace hez {
	extern float** mc2d_array;
	void mc2dInitialize(int width, int height, int cellWidth, int cellHeight);
	inline void mc2dSetValue(int x, int y, float val) {	mc2d_array[y][x] = val; }
	void mc2dDraw(color cl, float borderValue);
}

#endif
