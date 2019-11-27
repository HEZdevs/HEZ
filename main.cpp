#include <windows.h>
#include <sstream>
#include "vector.h"
#include "color.h"
using namespace hez;

#define WINDOW_CAPTION "HEZ"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FPS 50

//global
HBITMAP hbmp;
HANDLE hTickThread;
HWND hwnd;
HDC hdcMem;
color *pixels;

void drawFrame(color *pixels) {
  color *p;
  int yw;
  float px, py;
  for (int y = 0; y < WINDOW_HEIGHT; ++y) {
  	yw = y * WINDOW_WIDTH;
    for (int x = 0; x < WINDOW_WIDTH; ++x) {
      p = &pixels[yw + x];
      px = float(x) / float(WINDOW_WIDTH);
      py = float(y) / float(WINDOW_HEIGHT);
      p->r = (unsigned char)(px * 255);
      p->g = (unsigned char)(py * 255);
      p->b = 255;
    }
  }
}


DWORD WINAPI tickThreadProc(HANDLE handle) {
		vector v(1.0f, 1.0f, 0.0f); //вектор, который в двумерном пространстве как из 0,0 в 1,1. 
	v.normalize(); //нормализуем, длина = 1
	v = -v;
	std::stringstream ss;
	ss << v.x << " " << v.y << " " << v.z; // x и y == 1 делить на корень из 2 т.к. длина вектора изначально будет равна корню из 2 (как диагональ квадрата 1 на 1)
	MessageBox(hwnd, ss.str().c_str(), "Vector Test", 0);
	
  Sleep( 50 );
  ShowWindow( hwnd, SW_SHOW );
  HDC hdc = GetDC( hwnd );
  hdcMem = CreateCompatibleDC( hdc );
  HBITMAP hbmOld = (HBITMAP)SelectObject( hdcMem, hbmp );
  // Milliseconds to wait each frame
  int delay = 1000 / FPS;
  while(true) {
    // Do stuff with pixels
    drawFrame( pixels );
    // Draw pixels to window
    BitBlt( hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY );
    // Wait
    Sleep( delay );
  }
  SelectObject( hdcMem, hbmOld );
  DeleteDC( hdc );
}

void MakeSurface(HWND hwnd) {
  BITMAPINFO bmi;
  bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
  bmi.bmiHeader.biWidth = WINDOW_WIDTH;
  bmi.bmiHeader.biHeight =  -WINDOW_HEIGHT; // Order pixels from top to bottom
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32; // last byte not used, 32 bit for alignment
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = 0;
  bmi.bmiHeader.biXPelsPerMeter = 0;
  bmi.bmiHeader.biYPelsPerMeter = 0;
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biClrImportant = 0;
  bmi.bmiColors[0].rgbBlue = 0;
  bmi.bmiColors[0].rgbGreen = 0;
  bmi.bmiColors[0].rgbRed = 0;
  bmi.bmiColors[0].rgbReserved = 0;
  HDC hdc = GetDC( hwnd );
  // Create DIB section to always give direct access to pixels
  hbmp = CreateDIBSection( hdc, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0 );
  DeleteDC( hdc );
  // Create a new thread to use as a timer
  hTickThread = CreateThread( NULL, NULL, &tickThreadProc, NULL, NULL, NULL );
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_CREATE:
      	{
       		MakeSurface( hwnd );
      	}
      	break;
      
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		case WM_PAINT: {
	        PAINTSTRUCT ps;
	        HDC hdc = BeginPaint( hwnd, &ps );
	        BitBlt( hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY );
	        EndPaint( hwnd, &ps );
	    }
	    break;
		
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; 
	MSG msg; 
	
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; 
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass",WINDOW_CAPTION,WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		WINDOW_WIDTH, 
		WINDOW_HEIGHT, 
		NULL, NULL, hInstance, NULL);
	
	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}
	
	return msg.wParam;
}
