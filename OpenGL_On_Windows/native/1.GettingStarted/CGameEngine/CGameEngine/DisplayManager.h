#ifndef H_DISPLAYMANAGER
#define H_DISPLAYMANAGER

//header files
#include<iostream>
#include<stdio.h>
#include<vector>
#include<Windows.h>
#include"Logger.h"

#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"MathUtils.h"
#include"GameUtils.h"
#include"Camera.h"

//macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//CALBACK - _far_pascal or __stdcall
/*
	_far - keeps this function in OS reserved memory
	_pascal - follows pascal calling convention; method parameters reading order, who should remove activation records from stack (here callee)
*/
extern LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
extern void createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
extern void uninitializeWindow(void); // free up the resources

extern glm::mat4 gProjectionMatrix;
extern float gMixParam;

/*
Graphics applications and games usually keep track of a deltatime variable that stores the time
it takes to render the last frame. We then multiply all velocities with this deltaTime value.
The result is that when we have a large deltaTime in a frame, meaning that the last frame took longer
than average, the velocity for that frame will also be a bit higher to balance it all out.
When using this approach it does not matter if you have a very fast or slow pc, the velocity of the camera
will be balanced out accordingly so each user will have the same experience.
*/
extern long deltaTime;	// Time between current frame and last frame
#endif // !H_DISPLAYMANAGER