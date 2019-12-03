#include "vector.h"
#include "matrix.h"
#include "input.h"
#include "color.h"
#include "base.h"
#include "renderer.h"

#include <sstream>
//----------------------------------------------------MACRO---------------------------------------------------------//
#define DBG_MSG(x) { std::stringstream ss; ss << x; MessageBox(hwnd, ss.str().c_str(), "DBG_MSG", 0); }
#define SCREEN_SPACE_X(x) ((int)(((x)+1.0f) * WINDOW_WIDTH * 0.5f))
#define SCREEN_SPACE_Y(y) ((int)(((y)+1.0f) * WINDOW_HEIGHT * 0.5f))

namespace hez {
//--------------------------------------------------VARIABLES-------------------------------------------------------//
//winapi
HWND hwnd;
//settings
float fovRad = 0.7f; 
float zNearPlane = 0.1f;
float zFarPlane = 100.0f;
float camSpeed = 0.07f;
float camRotSpeed = 0.4f;
//camera & matrices
float mouseDeltaX = 0.0f, mouseDeltaY = 0.0f;
vector camPos = vector(0, 0, -1);
vector camUp = vector(0, 1, 0);
vector camDir = vector(0, 0, 1);
vector camRight = vector(1.0f, 0, 0);
vector camCurRight = camRight;
vector camCurDir = camDir;
vector camCurUp = camUp;
matrix camRotation;
matrix projectionMatrix;
matrix viewMatrix;
//helper
float screenCenterX = WINDOW_WIDTH / 2.0f;
float screenCenterY = WINDOW_HEIGHT / 2.0f;
//------------------------------------------------------------------------------------------------------------------//
//cube
vector cubePosition = vector(-0.5f, -0.5f, 3.0f);
vector cubeScale = vector(1.0f, 1.0f, 1.0f);
vector cubeRotation = vector(0.0f, 0.0f, 0.0f);
//cube:data
vector vertices[8] = { vector(0, 0, 0), vector(0,0,1), vector(1,0,1), vector(1,0,0), vector(1,1,0), vector(0,1,0), vector(0,1,1), vector(1,1,1) };
int faces[12][3] = { {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5}, {0, 1, 6}, {0, 6, 5}, {3, 2, 7}, {3, 7, 4}, {4, 5, 6}, {4, 6, 7}, {1, 2, 7}, {1, 7, 6} };

//--------------------------------------------------METHODS---------------------------------------------------------//
void line(const color& cl, vector a, vector b)
{
	a = viewMatrix.transform(a);
	b = viewMatrix.transform(b);
	a = projectionMatrix.transform(a);
	b = projectionMatrix.transform(b);
	rDrawLine(cl, SCREEN_SPACE_X(a.x), SCREEN_SPACE_Y(a.y), SCREEN_SPACE_X(b.x), SCREEN_SPACE_Y(b.y));
}
//------------------------------------------------BASE METHODS------------------------------------------------------//
void bInitialize(HWND _hwnd) {
	hwnd = _hwnd;
	rInitialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	projectionMatrix = matrix::createProjectionPerspective(fovRad, (float)WINDOW_WIDTH / WINDOW_HEIGHT, zNearPlane, zFarPlane);
}
void bUpdate() {
		//process input from keyboard
	  	if(isKeyDown(VK_W)) camPos = camPos +  camCurDir * camSpeed;     
		if(isKeyDown(VK_S)) camPos = camPos - camCurDir * camSpeed;
		if(isKeyDown(VK_A)) camPos = camPos +  camCurRight * camSpeed;
	  	if(isKeyDown(VK_D)) camPos = camPos - camCurRight * camSpeed;
	  	if(isKeyDown(VK_Q)) camPos = camPos + camCurUp * camSpeed;
	  	if(isKeyDown(VK_E)) camPos = camPos -  camCurUp * camSpeed;
	  	if(isKeyDown(VK_M)) PostMessage(hwnd, WM_CLOSE, NULL, NULL);
	  	//mouse lock & update mouse delta
	  	POINT mp;
	  	GetCursorPos(&mp);
	  	mouseDeltaX += camRotSpeed * (mp.x - screenCenterX) / (float)WINDOW_WIDTH;
	  	mouseDeltaY -= camRotSpeed * (mp.y - screenCenterY) / (float)WINDOW_HEIGHT;
	  	SetCursorPos((int)screenCenterX, (int)screenCenterY);
	  	//update camera matrix & view matrix + update camera vectors
	  	camRotation = matrix::createRotationX(mouseDeltaY) *  matrix::createRotationY(mouseDeltaX);
	  	camCurRight = camRotation.transform(camRight);
	  	camCurDir = camRotation.transform(camDir);
	  	camCurUp = camRotation.transform(camUp);
		viewMatrix = matrix::createLookAt(camPos, camPos + camCurDir, camCurUp);
}
void bDraw() {
	//clear screen
	rFill(color::rgba(0, 0, 0, 255)); 
	//draw cube
	matrix cubeWorld = matrix::createRotationX(cubeRotation.x) * matrix::createRotationY(cubeRotation.y) * matrix::createRotationZ(cubeRotation.z);
	cubeWorld = cubeWorld * matrix::createTranslation(cubePosition);
	for(int i = 0; i < 12; i++)
	{
		vector a = cubeWorld.transform(vertices[faces[i][0]]);
		vector b = cubeWorld.transform(vertices[faces[i][1]]);
		vector c = cubeWorld.transform(vertices[faces[i][2]]);
		line(color::rgba(0, 255, 0, 255),  a, b);
		line(color::rgba(0, 255, 0, 255),  b, c);
		line(color::rgba(0, 255, 0, 255),  c, a);
	}
}

}
