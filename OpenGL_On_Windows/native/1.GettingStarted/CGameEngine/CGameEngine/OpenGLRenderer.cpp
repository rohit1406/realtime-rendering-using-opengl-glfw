// header files

#include"OpenGLRenderer.h"
#include"DisplayManager.h"
#include"ShaderProgram.h"
#include"Models.h"
#include"OpenGLUtils.h"
#include"GameUtils.h"
#include"Loader.h"
#include"MathUtils.h"
#include"EntityRenderer.h"

using namespace std;

// function prototype declarations

// global variables
struct TexturedModel gBricksModelTexture;

// transformation matrices
glm::mat4 gModelMatrix;
glm::mat4 gViewMatrix;
extern glm::mat4 gProjectionMatrix;

extern struct Camera camera;

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

extern long deltaTime; // Time between current frame and last frame
long lastFrame = 0; // Time of last frame
extern long openGLInitializationTime; // time at which OpenGL is initialized

// initialize rendering
void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// open logger
	openFileLogger();

	// create window
	createWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	logStaticData("Window created successfully.");

	openGLInitializationTime = timeSinceEpochMillisec();

	// initialize OpenGL context
	initializeOpenGL();
	logStaticData("OpenGL context initialized successfully.");

	// prepare the rendering
	prepareOpenGLForRendering();
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

// initialize VAOs, VBOs, EBOs, Shader programs
void prepareOpenGLForRendering()
{
	logStaticData("Initializing GLEW");
	//GLEW initialization code for GLSL
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	logStaticData("GLEW initialized successfully.");

	// create projection matrix
	gProjectionMatrix = createPerspectiveProjectionMatrix(WIN_WIDTH, WIN_HEIGHT);

	// initialize camera
	initializeCamera();

	prepareEntityRenderer(gProjectionMatrix);

	// data
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
	};

	float cubeVertices[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int indices[] = {  // note that we start from 0!
	 0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	int numElements = sizeof(vertices)/sizeof(vertices[0]);
	int dimentions = 3;
	int numIndices = sizeof(indices) / sizeof(indices[0]);

	int numCubeElements = sizeof(cubeVertices) / sizeof(cubeVertices[0]);
	// load data to VAO
	//loadPositionDataToVAO(numElements, vertices, dimentions, &gTriangleModel);
	loadInterleavedDataToVAO(numCubeElements, cubeVertices, false, true, &gBricksModelTexture.rawModel);
	//loadInterleavedDataWithIndicesToVAO(numElements, vertices, true, true, numIndices, indices, &gBricksModelTexture.rawModel);
	// textured model
	const string bricksImageFile = IMAGES_RESOURCE_FILE_LOC + string("container.jpg");
	const string faceImageFile = IMAGES_RESOURCE_FILE_LOC + string("awesomeface.png");
	
	gBricksModelTexture.textureID = loadTexture(bricksImageFile);
	gBricksModelTexture.textureIDFace = loadTexture(faceImageFile);
	//loadPositionDataWithIndicesToVAO(numElements, vertices, numIndices, indices, dimentions, &gTriangleModel);

	for (unsigned int i = 0; i < 10; i++)
	{
		struct Entity entity;
		entity.texturedModel = gBricksModelTexture;
		entity.translate = cubePositions[i];
		entity.rotateX = 0;
		entity.rotateY = 0;
		entity.rotateZ = 0;
		entity.scale = 1;
		processEntity(entity);
	}

	// configure OpenGL states
	enableDepthTesting();

	// rough
	
}

// uninitialize OpenGL context
void uninitializeOpenGL(void)
{
	// local variables
	std::vector<GLuint>::size_type index;

	// clean up renderers
	cleanUpEntities();

	// delete shaders
	for (index = 0; index != gShaderList.size(); index++)
	{
		if(gShaderList[index])
			glDeleteShader(gShaderList[index]);
	}

	// delete shaders programs
	for (index = 0; index != gShaderProgramObjectList.size(); index++)
	{
		if(gShaderProgramObjectList[index])
			glDeleteProgram(gShaderProgramObjectList[index]);
	}

	// delete buffers
	for (index = 0; index != gVBOList.size(); index++)
	{
		if(gVBOList[index])
			glDeleteBuffers(1, &gVBOList[index]);
	}

	// delete VAOs
	for (index = 0; index != gVAOList.size(); index++)
	{
		if(gVAOList[index])
			glDeleteVertexArrays(1, &gVAOList[index]);
	}

	// delete textures
	for (index = 0; index != gTexturesList.size(); index++)
	{
		if (gTexturesList[index])
			glDeleteTextures(1, &gTexturesList[index]);
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

	// close logger
	closeFileLogger();
}


void display(void)
{
	// local variables
	

	//code
	long currentFrame = getTimeInSecondsSinceOpenGLIsInitialized();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// clear OpenGL color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// calculate camera offsets
	updateCameraPosition(gCameraPosition);
	
	// render ground
	renderEntity();

	//glFlush(); //removing this as not needed for double buffer; instead use below
	SwapBuffers(ghdc);
}