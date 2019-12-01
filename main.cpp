#include <windows.h>
#include <sstream>
#include "vector.h"
#include "color.h"
#include "matrix.h"
#include "renderer.h"
using namespace hez;

#define SCREEN_SPACE_X(x) ((int)(((x)+1.0f) * WINDOW_WIDTH * 0.5f))
#define SCREEN_SPACE_Y(y) ((int)(((y)+1.0f) * WINDOW_HEIGHT * 0.5f))
#define DBG_MSG(x) { std::stringstream ss; ss << x; MessageBox(hwnd, ss.str().c_str(), "DBG_MSG", 0); }

#define WINDOW_CAPTION "HEZ"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 50

//global
HBITMAP hbmp;
HANDLE hTickThread;
HWND hwnd;
HDC hdcMem;

float mouseDeltaX = 0.0f, mouseDeltaY = 0.0f;

//
vector camPos = vector::zero;
vector camUp = vector::unitY;
vector camDir = vector(0, 0, 1.0f);
vector camRight = vector(1.0f, 0, 0);

vector camCurRight = camRight;
vector camCurDir = camDir;
vector camCurUp = camUp;

vector cubePosition = vector(-0.5f, -0.5f, 3);
//vector cubeScale = vector(1.0f, 1.0f, 1.0f);
vector cubeRotation = vector(0.0f, 0.0f, 0.0f);

//
float fovRad = 0.7f; 
float zNearPlane = 0.1f;
float zFarPlane = 100.0f;
float camSpeed = 0.07f;
float camRotSpeed = 0.4f;
//
vector vertices[8] = { vector(0, 0, 0), vector(0,0,1), vector(1,0,1), vector(1,0,0), vector(1,1,0), vector(0,1,0), vector(0,1,1), vector(1,1,1) };
int faces[12][3] = { {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5}, {0, 1, 6}, {0, 6, 5}, {3, 2, 7}, {3, 7, 4}, {4, 5, 6}, {4, 6, 7}, {1, 2, 7}, {1, 7, 6} };

//
matrix projectionMatrix;
matrix viewMatrix;
matrix worldMatrix;
matrix camRotation;

void line(const color& cl, vector a, vector b)
{
	a = worldMatrix.transform(a);
	b = worldMatrix.transform(b);
	a = viewMatrix.transform(a);
	b = viewMatrix.transform(b);
	a = projectionMatrix.transform(a);
	b = projectionMatrix.transform(b);
	rDrawLine(cl, SCREEN_SPACE_X(a.x), SCREEN_SPACE_Y(a.y), SCREEN_SPACE_X(b.x), SCREEN_SPACE_Y(b.y));
}

void initRenderer()
{
	rInitialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	projectionMatrix = matrix::createProjectionPerspective(fovRad, (float)WINDOW_WIDTH / WINDOW_HEIGHT, zNearPlane, zFarPlane);
	worldMatrix = matrix::createTranslation(vector(0, 0, 3));
}

void drawFrame() {
  	rFill(color::rgba(120, 160, 220, 255));
  	matrix cubeWorld = matrix::createRotationX(cubeRotation.x) * matrix::createRotationY(cubeRotation.y) * matrix::createRotationZ(cubeRotation.z);
  	cubeWorld = cubeWorld * matrix::createTranslation(cubePosition);
  	for(int i = 0; i < 12; i++)
 	{
  		vector a = cubeWorld.transform(vertices[faces[i][0]]);
  		vector b = cubeWorld.transform(vertices[faces[i][1]]);
  		vector c = cubeWorld.transform(vertices[faces[i][2]]);
  		line(color::rgba(0, 255, 128, 255),  a, b);
  		line(color::rgba(0, 255, 128, 255),  b, c);
 	 	line(color::rgba(0, 255, 128, 255),  c, a);
  	}
}


DWORD WINAPI tickThreadProc(HANDLE handle) {
  Sleep( 50 );
  ShowWindow( hwnd, SW_SHOW );
  HDC hdc = GetDC( hwnd );
  hdcMem = CreateCompatibleDC( hdc );
  HBITMAP hbmOld = (HBITMAP)SelectObject( hdcMem, hbmp );
  int delay = 1000 / FPS;
  POINT lmp;
  GetCursorPos(&lmp);
  while(true) {
  	POINT mp;
  	GetCursorPos(&mp);
  	mouseDeltaX += camRotSpeed * (mp.x - lmp.x) / (float)WINDOW_WIDTH;
  	mouseDeltaY -= camRotSpeed * (mp.y - lmp.y) / (float)WINDOW_HEIGHT;
  	//
  	camRotation = matrix::createRotationX(mouseDeltaY) *  matrix::createRotationY(mouseDeltaX);
  	camCurRight = camRotation.transform(camRight);
  	camCurDir = camRotation.transform(camDir);
  	camCurUp = camRotation.transform(camUp);
	viewMatrix = matrix::createLookAt(camPos, camPos + camCurDir, camCurUp);
	//
    drawFrame( );
    BitBlt( hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY );
    Sleep( delay );
    lmp = mp;
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
       		MakeSurface( hwnd );
      		break;
      	case WM_KEYDOWN: 
      		switch(wParam) {
      			case VK_UP:
      				cubePosition.z += 0.5f;
      				break;
      			case VK_LEFT:
      				cubePosition.x -= 0.5f;
      				break;
      			case VK_DOWN: 
      				cubePosition.z -= 0.5f;
      				break;
      			case VK_RIGHT: 
      				cubePosition.x += 0.5f;
      				break;
      			case VK_SHIFT:
      				camPos.y -= 0.5f;
      				break;
			}
      		break;
      	case WM_CHAR:
      		switch(wParam) {
      		 	case 'w': camPos = camPos +  camCurDir * camSpeed;
				break;	
				case 's': camPos = camPos - camCurDir * camSpeed;
				break;
				case 'a': camPos = camPos +  camCurRight * camSpeed;
				break;	
				case 'd': camPos = camPos - camCurRight * camSpeed;
				break;
				case 'q': camPos = camPos + camCurUp * camSpeed;
				break;
				case 'e': camPos = camPos -  camCurUp * camSpeed;
				break;
			}
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
