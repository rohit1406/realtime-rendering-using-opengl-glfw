// header files

#include"OpenGLRenderer.h"
#include"DisplayManager.h"
#include"ShaderProgram.h"
#include"Models.h"
#include"OpenGLStateChanger.h"

using namespace std;

// function prototype declarations
extern void loadPositionDataToVAO(int dataSize, const float data[], int dimentsions, struct RawModel *rawModel);
extern void loadPositionDataWithIndicesToVAO(int numElements, const float data[], int indicesSize, const unsigned int indices[], int dimentsions, struct RawModel* rawModel);
extern vector<GLuint> giShaderList;
extern vector<GLuint> giShaderProgramObjectList;

// global variables
GLuint giEntityShaderProgram;
struct RawModel gLeftTriangleModel;
struct RawModel gRightTriangleModel;

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

	// prepare the rendering
	prepareOpenGLForRendering();
}

// render scene to display
void render(void)
{
	display(); // display our game
}

// initialize VAOs, VBOs, EBOs, Shader programs
void prepareOpenGLForRendering()
{
	//GLEW initialization code for GLSL
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// create shader program object
	giEntityShaderProgram = buildShaderProgramObject();

	// data
	const float verticesLeft[] = {
	/*0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left */
		// first triangle
		-0.5f, 0.5f, 0.0f,
		-0.75f, -0.75f, 0.0, 
		-0.25f, -0.25f, 0.0f
		
	};

	const float verticesRight[] = {
		
		// second triangle
		0.5f, 0.5f, 0.0f,
		0.25f, -0.25f, 0.0f,
		0.75f, -0.75f, 0.0

	};

	const unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};

	int numElements = sizeof(verticesLeft)/sizeof(verticesLeft[0]);
	int dimentions = 3;
	int numIndices = sizeof(indices) / sizeof(indices[0]);
	// load data to VAO
	loadPositionDataToVAO(numElements, verticesLeft, dimentions, &gLeftTriangleModel);

	numElements = sizeof(verticesRight) / sizeof(verticesRight[0]);
	loadPositionDataToVAO(numElements, verticesRight, dimentions, &gRightTriangleModel);
	//loadPositionDataWithIndicesToVAO(numElements, vertices, numIndices, indices, dimentions, &gTriangleModel);
}

// uninitialize OpenGL context
void uninitializeOpenGL(void)
{
	// local variables
	std::vector<GLuint>::size_type index;

	// delete shaders
	for (index = 0; index != giShaderList.size(); index++)
	{
		glDeleteShader(giShaderList[index]);
	}

	// delete shader program objects
	for (index = 0; index != giShaderProgramObjectList.size(); index++)
	{
		glDeleteProgram(giShaderProgramObjectList[index]);
	}

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

	startProgram(giEntityShaderProgram);
	// bind VAO
	glBindVertexArray(gLeftTriangleModel.vaoID);
	glEnableVertexAttribArray(0); // enable attribute 0
	glDrawArrays(GL_TRIANGLES, 0, gLeftTriangleModel.vertexCount);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for indexed drawing
	glDisableVertexAttribArray(0);// disable attribute 0
	
	//unbind VAO
	glBindVertexArray(0);


	// bind VAO
	glBindVertexArray(gRightTriangleModel.vaoID);
	glEnableVertexAttribArray(0); // enable attribute 0
	glDrawArrays(GL_TRIANGLES, 0, gRightTriangleModel.vertexCount);
	glDisableVertexAttribArray(0);// disable attribute 0

	//unbind VAO
	glBindVertexArray(0);
	stopProgram();
	
	//glFlush(); //removing this as not needed for double buffer; instead use below
	SwapBuffers(ghdc);
}