#include <cmath>
#include "vector.h"
#include "matrix.h"
#include "input.h"
#include "color.h"
#include "base.h"
#include "renderer.h"
#include "marching_cubes_2d.h"
#include <CL/opencl.h>
#include "shaders.h"

#include <sstream>
//----------------------------------------------------MACRO---------------------------------------------------------//
#define DBG(x) { std::stringstream ss; ss << x; MessageBox(hwnd, ss.str().c_str(), "DBG_MSG", 0); }
#define SCREEN_SPACE_X(x) ((int)(((x)+1.0f) * WINDOW_WIDTH * 0.5f))
#define SCREEN_SPACE_Y(y) ((int)(((y)+1.0f) * WINDOW_HEIGHT * 0.5f))

namespace hez {
//--------------------------------------------------VARIABLES-------------------------------------------------------//
//winapi
HWND hwnd;
/*
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
*/
cl_kernel kernel;
cl_float* memR;
cl_float* memG;
cl_float* memB;
cl_uint time = 0;
void compute() {
	cl_int width = WINDOW_WIDTH;
	cl_int height = WINDOW_HEIGHT;
	int ret;
	int mem_length = width * height;		
	cl_mem mem_objR = clCreateBuffer(shaders::context, CL_MEM_WRITE_ONLY, mem_length * sizeof(cl_float), NULL, &ret);
	cl_mem mem_objG = clCreateBuffer(shaders::context, CL_MEM_WRITE_ONLY, mem_length * sizeof(cl_float), NULL, &ret);
	cl_mem mem_objB = clCreateBuffer(shaders::context, CL_MEM_WRITE_ONLY, mem_length * sizeof(cl_float), NULL, &ret);
	//clEnqueueWriteBuffer(shaders::command_queue, mem_obj, CL_TRUE, 0, mem_length * sizeof(cl_float), mem, 0, NULL, NULL);
	
	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&mem_objR);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&mem_objG);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&mem_objB);
	clSetKernelArg(kernel, 3, sizeof(cl_int), (void *)&width);
	clSetKernelArg(kernel, 4, sizeof(cl_int), (void *)&height);
	clSetKernelArg(kernel, 5, sizeof(cl_int), (void *)&time);
		
	memR = (cl_float *)malloc(sizeof(cl_float) * mem_length);
	memG = (cl_float *)malloc(sizeof(cl_float) * mem_length);
	memB = (cl_float *)malloc(sizeof(cl_float) * mem_length);	
	
	size_t global_work_size[1] = { mem_length };
    clEnqueueNDRangeKernel(shaders::command_queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
    	
    clEnqueueReadBuffer(shaders::command_queue, mem_objR, CL_TRUE, 0, mem_length * sizeof(cl_float), memR, 0, NULL, NULL);
    clEnqueueReadBuffer(shaders::command_queue, mem_objG, CL_TRUE, 0, mem_length * sizeof(cl_float), memG, 0, NULL, NULL);
    clEnqueueReadBuffer(shaders::command_queue, mem_objB, CL_TRUE, 0, mem_length * sizeof(cl_float), memB, 0, NULL, NULL);
    
    clReleaseMemObject(mem_objR);
    clReleaseMemObject(mem_objG);
    clReleaseMemObject(mem_objB);
}
//------------------------------------------------BASE METHODS------------------------------------------------------//
void bInitialize(HWND _hwnd) {
	hwnd = _hwnd;
	//Позволяет выводить в консоль, полезно для отладки.
	//AllocConsole();
	//freopen("CONOUT$", "wb", stdout);
	//Инициализация renderer.h
	rInitialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//Инициализация shaders.h [OpenCL]
	shaders::initialize();
	
	shaders::HEZ_CL_SOURCE src = shaders::loadSourceFromFile((char*)"shader.cl");
	kernel = shaders::createKernelFromSource("func", src);
		
	//projectionMatrix = matrix::createProjectionPerspective(fovRad, (float)WINDOW_WIDTH / WINDOW_HEIGHT, zNearPlane, zFarPlane);
}
void bUpdate() {	
	/*//process input from keyboard
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
	viewMatrix = matrix::createLookAt(camPos, camPos + camCurDir, camCurUp);*/
}
void bDraw() {
	compute();
	time++;
	for(int i = 0; i < WINDOW_HEIGHT; i++)
	for(int j = 0; j < WINDOW_WIDTH; j++) 
	{
		int id = i * WINDOW_WIDTH + j;
		rSetPixelRaw(color::rgba((unsigned char)(memR[id] * 255.0f),(unsigned char)(memG[id] * 255.0f),(unsigned char)(memB[id] * 255.0f),255), id);
	}
	/*//clear screen
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
	}*/
}

}
