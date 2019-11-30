#include <windows.h>
#include <sstream>
#include "vector.h"
#include "color.h"
#include "matrix.h"
#include "renderer.h"
using namespace hez;

#define SCREEN_SPACE_X(x) ((int)(((x)+0.5f) * WINDOW_WIDTH))
#define SCREEN_SPACE_Y(y) ((int)(((y)+0.5f) * WINDOW_HEIGHT))
#define DBG_MSG(x) { std::stringstream ss; ss << x; MessageBox(hwnd, ss.str().c_str(), "DBG_MSG", 0); }

#define WINDOW_CAPTION "HEZ"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FPS 50

//global
HBITMAP hbmp;
HANDLE hTickThread;
HWND hwnd;
HDC hdcMem;

//
vector camPos = vector::zero;
vector camUp = vector::unitY;
vector camForward = vector::unitZ;
float fovRad = 2.0f; 
float zNearPlane = 0.1f;
float zFarPlane = 1000.0f;
//
vector v[2] = { vector::unitZ * 3, vector::unitZ * 3 };
int ci = 0;
void cic() { ci = ci == 0 ? 1 : 0; }

//rendering
matrix projectionMatrix;
matrix viewMatrix;

void line(const color& cl, vector a, vector b)
{
	a = viewMatrix.transform(a);
	b = viewMatrix.transform(b);
	a = projectionMatrix.transform(a);
	b = projectionMatrix.transform(b);
	rDrawLine(cl, SCREEN_SPACE_X(a.x), SCREEN_SPACE_Y(a.y), SCREEN_SPACE_X(b.x), SCREEN_SPACE_Y(b.y));
}

void initRenderer()
{
	rInitialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	projectionMatrix = matrix::createProjectionPerspective(fovRad, WINDOW_WIDTH / WINDOW_HEIGHT, zNearPlane, zFarPlane);
}

void drawFrame() {
	viewMatrix = matrix::createViewLookAt(camPos, v[1], camUp);
  rFill(color::rgba(120, 160, 220, 255));
  line(color::rgba(0, 255, 128, 255),  v[0], v[1]);
  //здесь можно рисовать используя
  //rSetPixel(cl, x, y);
  //rDrawLine(cl, x1, y1, x2, y2);
  //(x,y - int; cl - color)
}


DWORD WINAPI tickThreadProc(HANDLE handle) {
  Sleep( 50 );
  ShowWindow( hwnd, SW_SHOW );
  HDC hdc = GetDC( hwnd );
  hdcMem = CreateCompatibleDC( hdc );
  HBITMAP hbmOld = (HBITMAP)SelectObject( hdcMem, hbmp );
  int delay = 1000 / FPS;
  while(true) {
    drawFrame( );
    BitBlt( hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY );
    Sleep( delay );
  }
  SelectObject( hdcMem, hbmOld );
  DeleteDC( hdc );
}

void MakeSurface(HWND hwnd) {
  initRenderer(); //hez
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
  hbmp = CreateDIBSection( hdc, &bmi, DIB_RGB_COLORS, (void**)&colorBuffer, NULL, 0 );
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
      	case WM_KEYDOWN:
      	{
      		switch(wParam) {
      			case VK_UP: 
      				v[ci].y -= 0.05f;
      				break;
      			case VK_LEFT:
					v[ci].x -= 0.05f; 
      				break;
      			case VK_DOWN: 
      				v[ci].y += 0.05f;
      				break;
      			case VK_RIGHT: 
      				v[ci].x += 0.05f;
      				break;
      			case VK_SHIFT: 
      				cic();
      				break;
			}
      		break;
      	}
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
