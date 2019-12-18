#ifndef HEZ_RENDERER_H
#define HEZ_RENDERER_H

#include <algorithm>
#include "vector.h"
#include "color.h"

namespace hez {	
	extern const float DEPTH_MAX;
	extern int rtWidth;
	extern int rtHeight;
	extern int bufferSize;
	extern float* depthBuffer;
	extern color* colorBuffer;

	void rInitialize(int rtWidth, int rtHeight);
	inline void rSetPixel(const color& cl, int x, int y) {
		int i = rtWidth * y + x;
		if(i < 0 || i >= bufferSize) return;
		depthBuffer[i] = DEPTH_MAX;
		colorBuffer[i] = cl;
	}
	inline void rSetPixel(const color& cl, int x, int y, float z) {
		int i = rtWidth * y + x;
		if(i < 0 || i >= bufferSize) return;
		if(depthBuffer[i] > z) 
		{
			depthBuffer[i] = z;
			colorBuffer[i] = cl;
		}
	}
	inline void rSetPixelUnchecked(const color& cl, int x, int y) {
		int i = rtWidth * y + x;
		depthBuffer[i] = DEPTH_MAX;
		colorBuffer[i] = cl;
	}
	inline void rSetPixelUnchecked(const color& cl, int x, int y, float z) {
		int i = rtWidth * y + x;
		if(depthBuffer[i] > z) 
		{
			depthBuffer[i] = z;
			colorBuffer[i] = cl;
		}
	}
	inline void rSetPixelRaw(const color& cl, int i) {
		depthBuffer[i] = DEPTH_MAX;
		colorBuffer[i] = cl;
	}
	inline void rFill(const color& cl) {
		std::fill(colorBuffer, colorBuffer + bufferSize, cl);
		std::fill(depthBuffer, depthBuffer + bufferSize, DEPTH_MAX);
	}
	void rDrawLine(const color& cl, int x0, int y0, int x1, int y1);
}

#endif
