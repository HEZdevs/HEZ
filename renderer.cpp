#include <limits>
#include "color.h"

namespace hez {
	float **depthBuffer;
	color **colorBuffer;
	
	void initializeRenderer(int rtWidth, int rtHeight)
	{
		float maxFloat = std::numeric_limits<float>::max();
		depthBuffer = new float*[rtWidth];
		colorBuffer = new color*[rtWidth];
		for(int i = 0; i < rtWidth; i++) 
		{
			depthBuffer[i] = new float[rtHeight];
			colorBuffer[i] = new color[rtHeight];
			for(int j = 0; j < rtHeight; j++) 
			{
				depthBuffer[i][j] = maxFloat;
				colorBuffer[i][j] = color(255, 255, 255, 255);
			}
		}
	}
	
	void drawLine(int x1, int y1, int x2, int y2, const color& color)
	{
		//TODO: implement
	}
}
