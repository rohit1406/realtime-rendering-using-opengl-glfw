// header files
#include"OpenGLRenderer.h"
#include"DisplayManager.h"

// initialize rendering
void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// create window
	createWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// initialize OpenGL context
	initializeOpenGL();
}

// initialize OpenGL Context
void initializeOpenGL(void)
{
	//function prototypes
	void resize(int, int);

	//local variables
	//this structure describes the pixel format of the drawing surface
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	//code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	//initialization of the structure PIXELFORMATDESCRIPTOR
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); //size of this data structure
	pfd.nVersion = 1; //version of this data structure
	//specify properties of pixel buffer
	pfd.dwFlags = PFD_DRAW_TO_WINDOW //buffer can draw to window or device surface
		| PFD_SUPPORT_OPENGL //the buffer supports OpenGL drawing
		| PFD_DOUBLEBUFFER; //for double buffer
	pfd.iPixelType = PFD_TYPE_RGBA; //type of pixel data
	pfd.cColorBits = 32;  //number of color bitplanes; It considers only 24, ignores alpha
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	//DC of WM_PAINT can only paint client area, in order to paint whole window, we must use GetDC
	//Every window has it's only rendering context; Usually one viewport have one rendering context
	ghdc = GetDC(ghwnd);

	//attemp to match an appropriate pixel format supported by a device context
	//to a given pixel format specification
	iPixelFormatIndex = ChoosePixelFormat(
		ghdc, //device context to search for a best pixel format match
		&pfd); // pixel format for which a best match is sought

	if (iPixelFormatIndex == 0)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == false)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	ghrc = wglCreateContext(ghdc);

	if (ghrc == NULL)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if (wglMakeCurrent(ghdc, ghrc) == false)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//resize(WIN_WIDTH, WIN_HEIGHT); //not needed for double buffering
}

// render scene to display
void render(void)
{
	display(); // display our game
}

// uninitialize OpenGL context
void uninitializeOpenGL(void)
{
	// uninitialize window first
	uninitializeWindow();

	// uninitialize context
	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghrc);
	ghrc = NULL;

	ReleaseDC(ghwnd, ghdc);
	ghdc = NULL;

	DestroyWindow(ghwnd);
	ghwnd = NULL;
}

void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT);


	//glFlush(); //removing this as not needed for double buffer; instead use below
	SwapBuffers(ghdc);
}