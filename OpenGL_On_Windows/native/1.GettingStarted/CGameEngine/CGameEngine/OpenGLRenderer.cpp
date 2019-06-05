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
#include"SunRenderer.h"

using namespace std;

// function prototype declarations

// global variables
struct TexturedModel gBricksModelTexture;
struct TexturedModel gSun;
struct Light light;

// transformation matrices
glm::mat4 gModelMatrix;
glm::mat4 gViewMatrix;
extern glm::mat4 gProjectionMatrix;

extern struct Camera camera;

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  -20.0f),
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
GLfloat rotationAngle = 0.0f;

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
	prepareSunRenderer(gProjectionMatrix);

	// data
	/*float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
	};*/

	float vertices[] = {
	// positions			// normals
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
	//loadInterleavedDataToVAO(numCubeElements, cubeVertices, false, true, &gBricksModelTexture.rawModel);
	loadInterleavedDataPosColNorTexToVAO(numElements, vertices, false, true, false, &gBricksModelTexture.rawModel);
	//loadInterleavedDataWithIndicesToVAO(numElements, vertices, true, true, numIndices, indices, &gBricksModelTexture.rawModel);
	// textured model
	//const string bricksImageFile = IMAGES_RESOURCE_FILE_LOC + string("container.jpg");
	//const string faceImageFile = IMAGES_RESOURCE_FILE_LOC + string("awesomeface.png");
	
	//gBricksModelTexture.textureID = loadTexture(bricksImageFile);
	//gBricksModelTexture.textureIDFace = loadTexture(faceImageFile);
	//loadPositionDataWithIndicesToVAO(numElements, vertices, numIndices, indices, dimentions, &gTriangleModel);


	// Create Light entity
	loadInterleavedDataToVAO(numCubeElements, cubeVertices, false, true, &gSun.rawModel);
	setUpLight(&light, &gSun);
	processSun(light);
	

	// Create Other entities
	for (unsigned int i = 0; i < 10; i++)
	{
		struct Entity entity;
		entity.texturedModel = gBricksModelTexture;
		entity.translate = cubePositions[i];
		entity.rotateX = 0;
		entity.rotateY = 0;
		entity.rotateZ = 0;
		entity.scale = 1;
		if (i == 0)
		{
			// emerald
			entity.material.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
			entity.material.diffuse = glm::vec3(0.07568f,0.61424f, 0.07568f);
			entity.material.specular = glm::vec3(0.633f, 0.727811f,0.633f);
			entity.material.shininess = 0.6f;
		}else if (i == 1)
		{
			// pearl
			entity.material.ambient = glm::vec3(0.25f, 0.20725f, 0.20725f);
			entity.material.diffuse = glm::vec3(1.0f, 0.829f, 0.829f);
			entity.material.specular = glm::vec3(0.296648f, 0.296648f, 0.296648f);
			entity.material.shininess = 0.088f;
		}
		else if (i == 2)
		{
			// ruby
			entity.material.ambient = glm::vec3(0.1745f, 0.01175f, 0.01175f);
			entity.material.diffuse = glm::vec3(0.61424f, 0.04136f, 0.04136f);
			entity.material.specular = glm::vec3(0.727811f, 0.626959f, 0.626959f);
			entity.material.shininess = 0.6f;
		}
		else if (i == 3)
		{
			// chrome
			entity.material.ambient = glm::vec3(0.25f, 0.25f, 0.25f);
			entity.material.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
			entity.material.specular = glm::vec3(0.774597f, 0.774597f, 0.774597f);
			entity.material.shininess = 0.6f;
		}
		else if (i == 4)
		{
			// copper
			entity.material.ambient = glm::vec3(0.19125f, 0.0735f, 0.0225f);
			entity.material.diffuse = glm::vec3(0.7038f,0.27048f, 0.0828f);
			entity.material.specular = glm::vec3(0.256777f, 0.137622f, 0.086014f);
			entity.material.shininess = 0.1f;
		}
		else if (i == 5)
		{
			// gold
			entity.material.ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
			entity.material.diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
			entity.material.specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
			entity.material.shininess = 0.4f;
		}
		else if (i == 6)
		{
			// silver
			entity.material.ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f);
			entity.material.diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
			entity.material.specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);
			entity.material.shininess = 0.4f;
		}
		else if (i == 7)
		{
			// cyan rubber
			entity.material.ambient = glm::vec3(0.0f, 0.05f, 0.05f);
			entity.material.diffuse = glm::vec3(0.4f, 0.5f, 0.5f);
			entity.material.specular = glm::vec3(0.04, 0.7f, 0.7f);
			entity.material.shininess = 0.078125f;
		}
		else if (i == 8)
		{
			// yellow rubber
			entity.material.ambient = glm::vec3(0.05f, 0.05f, 0.0f);
			entity.material.diffuse = glm::vec3(0.5f, 0.5f, 0.4f);
			entity.material.specular = glm::vec3(0.7, 0.7f, 0.04f);
			entity.material.shininess = 0.078125f;
		}
		else if (i == 9)
		{
			// red plastic
			entity.material.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
			entity.material.diffuse = glm::vec3(0.5f, 0.0f, 0.0f);
			entity.material.specular = glm::vec3(0.7f, 0.6f, 0.6f);
			entity.material.shininess = 0.25f;
		}
		else
		{
			// white plastic
			entity.material.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
			entity.material.diffuse = glm::vec3(0.55f, 0.55f, 0.55f);
			entity.material.specular = glm::vec3(0.70f, 0.70f, 0.70f);
			entity.material.shininess = 0.25f;
		}
		processEntity(entity);
	}

	// configure OpenGL states
	enableDepthTesting();

	// rough
	
}

void display(void)
{
	// local variables
	rotationAngle += 0.1f;
	if (rotationAngle >= 360.0f)
	{
		rotationAngle = 0.0f;
	}

	//code
	long currentFrame = getTimeInSecondsSinceOpenGLIsInitialized();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// clear OpenGL color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// calculate camera offsets
	updateCameraPosition(gCameraPosition);
	increateEntityRotation(rotationAngle, rotationAngle, rotationAngle);
	// render
	renderEntity(light);
	renderSun();
	

	//glFlush(); //removing this as not needed for double buffer; instead use below
	SwapBuffers(ghdc);
}

// uninitialize OpenGL context
void uninitializeOpenGL(void)
{
	// local variables
	std::vector<GLuint>::size_type index;

	// clean up renderers
	cleanUpSun();
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