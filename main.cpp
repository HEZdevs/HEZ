#include <windows.h>
#include "libs/renderer.h"
#include "base.h"

//global
HBITMAP hbmp;
HANDLE hTickThread;
HWND hwnd;
HDC hdcMem;

DWORD WINAPI tickThreadProc(HANDLE handle) {
  Sleep( 50 );
  ShowWindow( hwnd, SW_SHOW );
  HDC hdc = GetDC( hwnd );
  hdcMem = CreateCompatibleDC( hdc );
  HBITMAP hbmOld = (HBITMAP)SelectObject( hdcMem, hbmp );
  int delay = 1000 / FPS;
  while(true) {
  	hez::bUpdate();
    hez::bDraw();
    BitBlt( hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY );
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
  hbmp = CreateDIBSection( hdc, &bmi, DIB_RGB_COLORS, (void**)&hez::colorBuffer, NULL, 0 );
  DeleteDC( hdc );
  // Create a new thread to use as a timer
  hTickThread = CreateThread( NULL, NULL, &tickThreadProc, NULL, NULL, NULL );
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_CREATE:
			hez::bInitialize(hwnd); 
       		MakeSurface( hwnd );
      		break;
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
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
	//wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	ShowCursor(NULL); //hides cursor
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
