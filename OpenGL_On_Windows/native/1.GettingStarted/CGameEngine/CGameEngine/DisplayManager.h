#ifndef H_DISPLAYMANAGER
#define H_DISPLAYMANAGER

//header files
#include<iostream>
#include<stdio.h>
#include<vector>
#include<Windows.h>

#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL

// headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

#endif // !H_DISPLAYMANAGER