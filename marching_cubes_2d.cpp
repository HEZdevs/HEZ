#include "renderer.h"
#include "color.h"
#define _mc2d_bool(x,y,v) (mc2d_array[(y)][(x)] >= (v))

namespace hez {
	struct mc2d_border_segment {
		int n;
		int* x;
		int* y;
		mc2d_border_segment(int count, int* xs, int* ys) {
			n = count;
			x = xs;
			y = ys;
		}
	};
	mc2d_border_segment borderVariations[16] = {
		mc2d_border_segment(0, 0, 0),													//0000
		mc2d_border_segment(1, new int[2]{-1, 0}, new int[2]{0, -1}),   				//0001
		mc2d_border_segment(1, new int[2]{-1, -2}, new int[2]{0, -1}),  				//0010
		mc2d_border_segment(1, new int[2]{0, -2}, new int[2]{-1, -1}),   				//0011
		mc2d_border_segment(1, new int[2]{-2, -1}, new int[2]{-1, -2}),  				//0100
		mc2d_border_segment(1, new int[4]{-1, -2, 0, -1}, new int[4]{0, -1, -1, -2}),   //0101   
		mc2d_border_segment(1, new int[2]{-1, -1}, new int[2]{0, -2}),   				//0110
		mc2d_border_segment(1, new int[2]{0, -1}, new int[2]{-1, -2}),   				//0111
		mc2d_border_segment(1, new int[2]{0, -1}, new int[2]{-1, -2}),   				//1000 
		mc2d_border_segment(1, new int[2]{-1, -1}, new int[2]{0, -2}),   				//1001
		mc2d_border_segment(2, new int[4]{-1, 0, -2, -1}, new int[4]{0, -1, -1, -2}),   //1010 
		mc2d_border_segment(1, new int[2]{-2, -1}, new int[2]{-1, -2}),   				//1011 
		mc2d_border_segment(1, new int[2]{-2, 0}, new int[2]{-1, -1}),   				//1100 
		mc2d_border_segment(1, new int[2]{-1, -2}, new int[2]{0, -1}),   				//1101
		mc2d_border_segment(1, new int[2]{-1, 0}, new int[2]{0, -1}),   				//1110 
		mc2d_border_segment(0, 0, 0),   												//1111
	};	
	float** mc2d_array;
	int mc2d_w, mc2d_h, mc2d_hcw, mc2d_hch;
	void mc2dInitialize(int width, int height, int cellWidth, int cellHeight) {
		mc2d_w = width;
		mc2d_h = height;
		mc2d_hcw = cellWidth / 2;
		mc2d_hch = cellHeight / 2;
		mc2d_array = new float*[height];
		for(int i = 0; i < height; i++) mc2d_array[i] = new float[width];
	}
	void mc2dDraw(color cl, float val) {
		for(int x = 1; x < mc2d_w; x++)
			for(int y = 1; y < mc2d_h; y++) {
				mc2d_border_segment* seg = &borderVariations[ _mc2d_bool(x,     y,     val)        | 
															 (_mc2d_bool(x - 1, y,     val)  << 1) | 
															 (_mc2d_bool(x - 1, y - 1, val)  << 2) | 
															 (_mc2d_bool(x,     y - 1, val)  << 3) ];
				if(seg->n > 0) rDrawLine(cl, (seg->x[0] + 2 * x) * mc2d_hcw, (seg->y[0] + 2 * y) * mc2d_hch, (seg->x[1] + 2 * x) * mc2d_hcw, (seg->y[1] + 2 * y) * mc2d_hch);
				if(seg->n > 1) rDrawLine(cl, (seg->x[2] + 2 * x) * mc2d_hcw, (seg->y[2] + 2 * y) * mc2d_hch, (seg->x[3] + 2 * x) * mc2d_hcw, (seg->y[3] + 2 * y) * mc2d_hch);
			}
	}
}
