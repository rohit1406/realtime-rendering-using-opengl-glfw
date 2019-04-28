/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 27 December 2018

	Program Name - SSAO - Screen-Space Ambient Occlusion

	Important Notes:
	We can register the callback functions after we have created the window and before gameloop is initiated

	The output of the vertex shader requires the coordinates to be in clip-space which is what we just did with 
	the transformation matrices. OpenGL then performs perspective division on the clip-space coordinates to transform 
	them to normalized-device coordinates. OpenGL then uses the parameters from glViewPort to map the normalized-device 
	coordinates to screen coordinates where each coordinate corresponds to a point on your screen (in our case a 800x600 screen). 
	This process is called the viewport transform.
*/

//header files

#include "Include\glad\glad.h" //glad.h should be before glfw3.h; it contains correct OpenGL headers includes
#include "Include\GLFW\glfw3.h"


//OpenGL Mathematics (GLM) library
#include "Include\glm\glm.hpp"
#include "Include\glm\gtc\matrix_transform.hpp"
#include "Include\glm\gtc\type_ptr.hpp"

//to manage shaders and program objects
#include "shader.h"
//our camera object
#include "camera.h"

//to load the model
#include "model.h"
//#include "filesystem.h"
#include<iostream>
#include<random>

//global function declarations
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float lerp(float a, float b, float f);

//macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define VIEWPORT_WIDTH 800
#define VIEWPORT_HEIGHT 600

//global variable declaration
//to transform all the vertices to the globle world space
glm::mat4 gModelMatrix; //it contains translation, rotation and scalling

glm::mat4 gViewMatrix; //translate the scene towards -ve z axis, this gives impression that we are moving backwards

glm::mat4 gPerspectiveProjectionMatrix; //for perspective projection

float deltaTime = 0.0f; //time between current frame and last frame
float lastFrame = 0.0f; //time of last frame

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f) //position
				);
//mouse movements
bool firstMouse = true;
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;

//fullscreen
bool gbFullScreen = false;


//lighting info
const unsigned int NR_LIGHTS = 32;
glm::vec3 gvLightPosition;
glm::vec3 gvLightColor;

//FBO,VAO,VBO,CBO,DBO,RBO
unsigned int gBufferFBO; //G-Buffer framebuffer
unsigned int gPositionCBO, gNormalCBO, gAlbedoSpecCBO; //position, normal, specular color buffers attachments
unsigned int gDepthRBO; //depth attachment
unsigned int gQuadVAO, gQuadVBO; //for Quad rendering
unsigned int gCubeVAO, gCubeVBO; //for Cube rendering
unsigned int gSSAOFBO, gSSAOColorBuffer; //for SSAO
unsigned int gSSAOBlurFBO, gSSAOBlurColorBuffer; //for SSAO Blur

//attribute locations
enum
{
	RSM_POSITION_ATTRIBUTE = 0,
	RSM_NORMAL_ATTRIBUTE,
	RSM_TEXCOORD_ATTRIBUTE0
};


//entry-point function
int main()
{
	// function prototype declaration
	// -------------------------------
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);

	// code
	// -----
	//instantiate GLFW window
	glfwInit();

	// configure GLFW
	// ---------------
	//set core profile version as 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, //OpenGL major version
					3 //sets the value for the OpenGL major version
					);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, //OpenGL minor version
						3 //sets the value for the OpenGL minor version
					);

	//this ensures that we need access to the smaller subset of OpenGL features
	glfwWindowHint(GLFW_OPENGL_PROFILE, //what option we need to configure
				GLFW_OPENGL_CORE_PROFILE //sets the value for the choosen option
					);

	//glfwWindowHint(GLFW_AUTO_ICONIFY, true);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //this is neede for MAC only

	// Create Window Object
	// ---------------------
	GLFWwindow* window = glfwCreateWindow(
									WIN_WIDTH, //width
									WIN_HEIGHT, //height
									"OpenGL: SSAO", //title
									nullptr, //monitor
									NULL //share
								);


	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); //terminate the program
		return -1;
	}

	//bring window to center
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int xPos = (mode->width / 2) - (WIN_WIDTH/2);
	int yPos = (mode->height / 2) - (WIN_HEIGHT/2);
	glfwSetWindowPos(window, xPos, yPos);

	//make context of our window the main context on the current thread
	glfwMakeContextCurrent(window);

	//register mouse and scroll callbacks
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Initialize GLAD - load all OpenGL function pointers
	// ----------------------------------------------------
	//we pass GLAD the function to load the address of the OpenGL function pointers which is OS specific
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/*****************Build And Compile Our Shader Program Starts********************/
	Shader geometryPassShader("geometry_pass.vs", "geometry_pass.fs");
	Shader ssaoShader("ssao.vs", "ssao.fs");
	Shader ssaoBlurShader("ssao_blur.vs", "ssao_blur.fs");
	Shader lightingPassShader("lighting_pass.vs", "lighting_pass.fs");
	/*****************Build And Compile Our Shader Program Ends********************/

	/********Initialize some data starts************/
	
	//lighting info
	gvLightPosition = glm::vec3(2.0f, 4.0f, -2.0f);
	gvLightColor = glm::vec3(0.2f, 0.2f, 0.7f);

	float quadVertices[] =
	{
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	float cubeVertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };

	ssaoShader.use();
	ssaoShader.setInt("u_sampler_gPositionCBO", 0);
	ssaoShader.setInt("u_sampler_gNormalCBO", 1);
	ssaoShader.setInt("u_sampler_texNoise", 2);

	ssaoBlurShader.use();
	ssaoBlurShader.setInt("u_sampler_ssao_input", 0);

	lightingPassShader.use();
	lightingPassShader.setInt("u_sampler_gPositionCBO", 0);
	lightingPassShader.setInt("u_sampler_gNormalCBO", 1);
	lightingPassShader.setInt("u_sampler_gAlbedoSpecCBO", 2);
	lightingPassShader.setInt("u_sampler_ssao", 3);

	//Generate Sample Kernel
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); //generates random floats from 0 to 1
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;
	for (unsigned int i = 0; i < 64; i++)
	{
		//x and y varry between -1 to 1 and z varry between 0 to 1; if we varry between -1 to 1, we will have
		//a sphere sample kernel not hemisphere
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) 
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);

		float scale = float(i) / 64.0;

		//scale samples such that they are more alligned to the center of the kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	//generate noise texture
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		//rotate around z axis in tangent space
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0, 
			0.0f
		);
		ssaoNoise.push_back(noise);
	}
	/********Initialize some data ends************/
	cout << "Shaders Loaded" << endl;
	//load model
	cout << "Model Loading started" << endl;
	//Model ourModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
	Model ourModel("resources/objects/nanosuit/nanosuit.obj");
	//Model ourModel("Cube.dae");
	cout << "Model Loaded" << endl;

	/**********Configure FBO, VAO, VBO, CBO, etc starts*************/
	//create Framebuffer
	glGenFramebuffers(1, &gBufferFBO);

	//bind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);

	//create and attach position, normal, specular, depth attachments
	//position color buffer
	glGenTextures(1, &gPositionCBO);
	glBindTexture(GL_TEXTURE_2D, gPositionCBO);
	glTexImage2D(
		GL_TEXTURE_2D, //target
		0, //mipmap level
		GL_RGB16F, //internal format
		WIN_WIDTH, //width
		WIN_HEIGHT, //height
		0, //border
		GL_RGB, //image format
		GL_FLOAT, //type of image data
		NULL //actual data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionCBO, 0);

	//normal color buffer
	glGenTextures(1, &gNormalCBO);
	glBindTexture(GL_TEXTURE_2D, gNormalCBO);
	glTexImage2D(
		GL_TEXTURE_2D, //target
		0, //mipmap level
		GL_RGB16F, //internal format
		WIN_WIDTH, //width
		WIN_HEIGHT, //height
		0, //border
		GL_RGB, //image format
		GL_FLOAT, //type of image data
		NULL //actual data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormalCBO, 0);

	//specular color buffer
	glGenTextures(1, &gAlbedoSpecCBO);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpecCBO);
	glTexImage2D(
		GL_TEXTURE_2D, //target
		0, //mipmap level
		GL_RGB, //internal format
		WIN_WIDTH, //width
		WIN_HEIGHT, //height
		0, //border
		GL_RGB, //image format
		GL_UNSIGNED_BYTE, //type of image data
		NULL //actual data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpecCBO, 0);

	//tell OpenGL which color attachments of this framebuffer we will use while rendering
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	//Depth buffer attachment
	glGenRenderbuffers(1, &gDepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, gDepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIN_WIDTH, WIN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepthRBO);

	//finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR:: Framebuffer is not complete" << std::endl;
	}

	//unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//SSAO Framebuffer
	//create Framebuffer
	glGenFramebuffers(1, &gSSAOFBO);

	//bind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, gSSAOFBO);

	//create and attach position, normal, specular, depth attachments
	//position color buffer
	glGenTextures(1, &gSSAOColorBuffer);
	glBindTexture(GL_TEXTURE_2D, gSSAOColorBuffer);
	glTexImage2D(
		GL_TEXTURE_2D, //target
		0, //mipmap level
		GL_RED, //internal format
		WIN_WIDTH, //width
		WIN_HEIGHT, //height
		0, //border
		GL_RGB, //image format
		GL_FLOAT, //type of image data
		NULL //actual data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gSSAOColorBuffer, 0);

	//finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR:: SSAO Framebuffer is not complete" << std::endl;
	}

	//unbind SSAO framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//SSAO Blur Framebuffer
	//create Framebuffer
	glGenFramebuffers(1, &gSSAOBlurFBO);

	//bind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, gSSAOBlurFBO);

	//create and attach position, normal, specular, depth attachments
	//position color buffer
	glGenTextures(1, &gSSAOBlurColorBuffer);
	glBindTexture(GL_TEXTURE_2D, gSSAOBlurColorBuffer);
	glTexImage2D(
		GL_TEXTURE_2D, //target
		0, //mipmap level
		GL_RED, //internal format
		WIN_WIDTH, //width
		WIN_HEIGHT, //height
		0, //border
		GL_RGB, //image format
		GL_FLOAT, //type of image data
		NULL //actual data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gSSAOBlurColorBuffer, 0);

	//finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR:: SSAO Blur Framebuffer is not complete" << std::endl;
	}

	//unbind SSAO framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//configure for cube
	glGenVertexArrays(1, &gCubeVAO); //create VAO
	glBindVertexArray(gCubeVAO); //bind VAO

	glGenBuffers(1, &gCubeVBO); //create VBO
	glBindBuffer(GL_ARRAY_BUFFER, gCubeVBO); //bind VBO
	glBufferData(
		GL_ARRAY_BUFFER, //target
		sizeof(cubeVertices), //size
		cubeVertices, //data
		GL_STATIC_DRAW //usage
	);
	//tell OpenGL shader how to use the above data
	glVertexAttribPointer(
		RSM_POSITION_ATTRIBUTE, //0th in parameter
		3, //made of 3 values
		GL_FLOAT, //of type float
		GL_FALSE, //which are not necessary to normalize
		8 * sizeof(float), //stride
		(void *) 0 //offset
	);
	glEnableVertexAttribArray(RSM_POSITION_ATTRIBUTE);

	glVertexAttribPointer(
		RSM_NORMAL_ATTRIBUTE, //1st in parameter
		3, //made of 3 values
		GL_FLOAT, //of type float
		GL_FALSE, //which are not necessary to normalize
		8 * sizeof(float), //stride
		(void *)(3 * sizeof(float)) //offset
	);
	glEnableVertexAttribArray(RSM_NORMAL_ATTRIBUTE);

	glVertexAttribPointer(
		RSM_TEXCOORD_ATTRIBUTE0, //2nd in parameter
		2, //made of 2 values
		GL_FLOAT, //of type float
		GL_FALSE, //which are not necessary to normalize
		8 * sizeof(float), //stride
		(void *) (6 * sizeof(float)) //offset
	);
	glEnableVertexAttribArray(RSM_TEXCOORD_ATTRIBUTE0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO
	glBindVertexArray(0); //unbind VAO



	//configure for quad
	glGenVertexArrays(1, &gQuadVAO); //create VAO
	glBindVertexArray(gQuadVAO); //bind VAO

	glGenBuffers(1, &gQuadVBO); //create VBO
	glBindBuffer(GL_ARRAY_BUFFER, gQuadVBO); //bind VBO
	glBufferData(
		GL_ARRAY_BUFFER, //target
		sizeof(quadVertices), //size
		&quadVertices, //data
		GL_STATIC_DRAW //usage
	);
	//tell OpenGL shader how to use the above data
	glVertexAttribPointer(
		RSM_POSITION_ATTRIBUTE, //0th in parameter
		3, //made of 3 values
		GL_FLOAT, //of type float
		GL_FALSE, //which are not necessary to normalize
		5 * sizeof(float), //stride
		(void *)0 //offset
	);
	glEnableVertexAttribArray(RSM_POSITION_ATTRIBUTE);

	glVertexAttribPointer(
		1, //0th in parameter
		2, //made of 2 values
		GL_FLOAT, //of type float
		GL_FALSE, //which are not necessary to normalize
		5 * sizeof(float), //stride
		(void *)(3 * sizeof(float)) //offset
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO
	glBindVertexArray(0); //unbind VAO

	//create 4*4 texture to hold the rotation vectors
	unsigned int noiseTexture;
	glGenTextures(1, &noiseTexture); //create texture
	glBindTexture(GL_TEXTURE_2D, noiseTexture); //bind texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]); //pass data to texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture
	/**********Configure FBO, VAO, VBO, CBO, etc ends*************/

	/*******Initialize some params**********/
	//enable depth testing
	glEnable(GL_DEPTH_TEST);

	//glDisable(GL_BLEND);
	//tell OpenGL the size of the rendering window
	glViewport(0, 0,  //location of the lower left corner of the window
			VIEWPORT_WIDTH, VIEWPORT_HEIGHT //width and height of the rendering window
		);

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//register framebuffer_size_callback for adjusting viewport according to the window resizing
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// render loop
	// ------------
	while (!glfwWindowShouldClose(window)) //check if GLFW has been instructed to close
	{
		// input
		//-------
		processInput(window);

		//calculate new delta time value within each frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// rendering commands here
		// -----------------------
		//at the start of each iteration, clear the screen with color of your choice
		//else we should see the results from previous iteration
		//entire color buffer will be filled with this color
		glClearColor(0.1f, 0.3f, 0.1f, 1.0f);

		/*****1. Geometry Pass: render scenes geometry/color data to gbuffer***************/
		//bind framebuffer to render to
		glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
		//clear the screens color buffer
		//here we pass in buffer bits to specify which buffer we would like to clear
		glClear(GL_COLOR_BUFFER_BIT //clear color buffer
				| GL_DEPTH_BUFFER_BIT //clear depth buffer
				);
		
		/**********CREATE TRANSFORMATION MATRICES***************/
		//load transformation matrices to identity matrix
		gModelMatrix = glm::mat4();
		gViewMatrix = glm::mat4();
		gPerspectiveProjectionMatrix = glm::mat4();

		//view transformation
		gViewMatrix = camera.GetViewMatrix();

		//perspective transformation
		gPerspectiveProjectionMatrix = glm::perspective(
											glm::radians(camera.Zoom), //fovy
											(float)WIN_WIDTH/(float)WIN_HEIGHT, //aspect ratio
											0.1f, //near plane distance from camera
											50.0f //far plane distance from near plane
											);

		//activate shader program object
		geometryPassShader.use();
		//pass values to uniform
		geometryPassShader.setMat4("u_view_matrix", gViewMatrix);
		geometryPassShader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);

		//model transformation
		gModelMatrix = glm::mat4(); //make identity matrix

		//model transformation
		glm::vec3 translate_object_vec = glm::vec3(0.0f, 7.0f, 0.0f);
		gModelMatrix = glm::translate(gModelMatrix,
			translate_object_vec //translate by
		);

		gModelMatrix = glm::scale(gModelMatrix,
			glm::vec3(7.5f, 7.5f, 7.5f) //scale by
		);

		geometryPassShader.setMat4("u_model_matrix", gModelMatrix);
		geometryPassShader.setInt("u_invert_normals", 1); //invert normals as we are in the cube
		//render cube
		glBindVertexArray(gCubeVAO); //bind cube vao
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0); //unbind cube vao
		geometryPassShader.setInt("u_invert_normals", 0);
		//model on the floor
		gModelMatrix = glm::mat4(); //make identity matrix
		//model transformation
		translate_object_vec = glm::vec3(0.0f, 0.0f, 5.0f);
		gModelMatrix = glm::translate(gModelMatrix,
			translate_object_vec //translate by
		);
		gModelMatrix = glm::rotate(gModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate
		gModelMatrix = glm::scale(gModelMatrix,
			glm::vec3(0.5f) //scale by
		);
		geometryPassShader.setMat4("u_model_matrix", gModelMatrix);
		//draw
		ourModel.Draw(geometryPassShader);
			
		
		//unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		/******Geometry Pass Ends*********/

		/*
			2. Generate SSAO Texture
		*/
		glBindFramebuffer(GL_FRAMEBUFFER, gSSAOFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		ssaoShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPositionCBO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormalCBO);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		//sent kernel + rotation
		for (unsigned int i = 0; i < 64; i++)
		{
			ssaoShader.setVec3("u_samples["+std::to_string(i)+"]", ssaoKernel[i]);
		}
		ssaoShader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);
		//render quad
		glBindVertexArray(gQuadVAO); //bind necessary VAO
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0); //unbind VAO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//generation of SSAO texture ends

		/*
			3.Blur SSAO Texture to remove noise
		*/
		glBindFramebuffer(GL_FRAMEBUFFER, gSSAOBlurFBO); //bind
		glClear(GL_COLOR_BUFFER_BIT);
		ssaoBlurShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gSSAOColorBuffer);
		//render quad
		glBindVertexArray(gQuadVAO); //bind necessary VAO
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0); //unbind VAO
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind

		/*
			4. Lighting Pass: calculate lighting by iterating over screen filled with quad pixel-by-pixel using the gbuffers contents
			-------------------------------------------------------------------------------------------------------------------------
		*/
		//clear the screens color buffer
		//here we pass in buffer bits to specify which buffer we would like to clear
		glClear(GL_COLOR_BUFFER_BIT //clear color buffer
			| GL_DEPTH_BUFFER_BIT //clear depth buffer
		);

		lightingPassShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPositionCBO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormalCBO);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpecCBO);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gSSAOBlurColorBuffer);

		//set light related uniforms
		glm::vec3 lightPosView = glm::vec3(camera.GetViewMatrix() * glm::vec4(gvLightPosition, 1.0));
		lightingPassShader.setVec3("u_lights.Position", lightPosView);
		lightingPassShader.setVec3("u_lights.Color", gvLightColor);

		//update attenuation params
		const float constant = 1.0;
		const float linear = 0.09;
		const float quadratic = 0.032;
		lightingPassShader.setFloat("u_lights.Linear", linear);
		lightingPassShader.setFloat("u_lights.Quadratic", quadratic);
		
		//render quad
		glBindVertexArray(gQuadVAO); //bind necessary VAO
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0); //unbind VAO

			
		//swap color buffers - a large buffer that contains the color values for each pixel's in GLFW window
		//that has been used to draw in during this iteration and show it as the output to the screen
		glfwSwapBuffers(window);

		//check if any events are triggered
		glfwPollEvents();
	}

	//glfw: clean/delete all GLFW's resources that were allocated
	glfwTerminate();

	return 0;
}

/*  framebuffer_size_callback: callback function on the window that gets called each time the window is resized.
	Because, the moment user resizes the window, the viewport should be adjusted as well
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//tell OpenGL the size of the rendering window
	glViewport(0, 0,  //location of the lower left corner of the window
		width, height //width and height of the rendering window
	);
}

/* processInput: query GLFW whether relevant keys are pressed/released this frame and react accordingly*/
void processInput(GLFWwindow* window)
{
	//if escape key is pressed; if not then GLFW_RELEASE is returned
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	//adjust camera speed
	float cameraSpeed = 2.5f * deltaTime;
	//go forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	//come backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	//go left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	//go right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	//Full screen
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		if (gbFullScreen == false)
		{
			gbFullScreen = true;
			
			glfwSetWindowSize(window, mode->width, mode->height);
			glfwSetWindowPos(window, 0, 0);
		}
		else
		{
			gbFullScreen = false;
			glfwSetWindowSize(window, WIN_WIDTH, WIN_HEIGHT);
			int xPos = (mode->width / 2) - (WIN_WIDTH / 2);
			int yPos = (mode->height / 2) - (WIN_HEIGHT / 2);
			glfwSetWindowPos(window, xPos, yPos);
		}
	}
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	//if first time mouse is entring in the window
	//this is to avoid sudden jump to the entrace point of the mouse pointer
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	//calculate the offset from the previouse mouse pointer position to the next mouse pointer position in this frame
	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	//process these offsets
	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}