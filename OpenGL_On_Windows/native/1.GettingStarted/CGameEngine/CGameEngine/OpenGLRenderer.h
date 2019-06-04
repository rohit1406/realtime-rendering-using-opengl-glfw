#ifndef H_OPENGLRENDERER
#define H_OPENGLRENDERER

//header files
#include <Windows.h>
#include"Logger.h"

// global variable declarations
extern HDC ghdc;
extern HGLRC ghrc;
extern HWND ghwnd;

// global variables
extern bool gbActiveWindow;
extern bool gbEscapeKeyPressed;
extern bool gbDone;
extern bool gbFullscreen;

// function prototype declarations
extern void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
extern void initializeOpenGL(void);
extern void prepareOpenGLForRendering(); // initialize VAOs, VBOs, EBOs, Shader programs
extern void uninitializeOpenGL(void);
extern void render(void);
void display(void); // display our game

#endif // !H_OPENGLRENDERER
