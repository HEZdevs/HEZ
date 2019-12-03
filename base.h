#ifndef HEZ_BASE_H
#define HEZ_BASE_H

//window properties
#define WINDOW_CAPTION "HEZ"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define FPS 50

namespace hez {
	void bInitialize(HWND _hwnd);
	void bUpdate();
	void bDraw();
}

#endif
