#include <limits>
#include "color.h"
#include "renderer.h"

namespace hez {
	const float DEPTH_MAX = std::numeric_limits<float>::max();
	int rtWidth, rtHeight;
	int bufferSize;
	float* depthBuffer;
	color* colorBuffer;
	
	void rInitialize(int _rtWidth, int _rtHeight)
	{
		rtWidth = _rtWidth;
		rtHeight = _rtHeight;
		bufferSize = _rtWidth * _rtHeight;
		depthBuffer = new float[bufferSize];
		colorBuffer = new color[bufferSize];
		rFill(rgba(0, 0, 0, 255));
	}
	//https://ru.wikibooks.org/wiki/%D0%A0%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D0%B8_%D0%B0%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC%D0%BE%D0%B2/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%91%D1%80%D0%B5%D0%B7%D0%B5%D0%BD%D1%85%D1%8D%D0%BC%D0%B0#%D0%A0%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D1%8F_%D0%BD%D0%B0_C++
	void rDrawLine(const color& cl, int x1, int y1, int x2, int y2)
	{
		const int deltaX = abs(x2 - x1);
	    const int deltaY = abs(y2 - y1);
	    const int signX = x1 < x2 ? 1 : -1;
	    const int signY = y1 < y2 ? 1 : -1;
	    int error = deltaX - deltaY;
	    rSetPixel(cl, x2, y2);
	    while(x1 != x2 || y1 != y2) 
	   	{
	        rSetPixel(cl, x1, y1);
	        const int error2 = error * 2;
	        if(error2 > -deltaY) 
	        {
	            error -= deltaY;
	            x1 += signX;
	        }
	        if(error2 < deltaX) 
	        {
	            error += deltaX;
	            y1 += signY;
	        }
	    }
	}
}
