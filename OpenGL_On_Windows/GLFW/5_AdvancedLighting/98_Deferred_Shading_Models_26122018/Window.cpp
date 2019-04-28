/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 26 December 2018

	Program Name - Deferred Shading on Model Loading using Assimp - Open Asset Import Library 

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

//global function declarations
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f) //position
				);
//mouse movements
bool firstMouse = true;
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;

//fullscreen
bool gbFullScreen = false;

//object positions
std::vector<glm::vec3> gvObjectPositions;

//lighting info
const unsigned int NR_LIGHTS = 32;
std::vector<glm::vec3> gvLightPositions;
std::vector<glm::vec3> gvLightColors;

//FBO,VAO,VBO,CBO,DBO,RBO
unsigned int gBufferFBO; //G-Buffer framebuffer
unsigned int gPositionCBO, gNormalCBO, gAlbedoSpecCBO; //position, normal, specular color buffers attachments
unsigned int gDepthRBO; //depth attachment
unsigned int gQuadVAO, gQuadVBO; //for Quad rendering
unsigned int gCubeVAO, gCubeVBO; //for Cube rendering

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
									"OpenGL: Rectangle", //title
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
	Shader geometryPassShader("vertex_shader.vs", "fragment_shader.fs");
	Shader lightingPassShader("lighting_pass.vs", "lighting_pass.fs");
	Shader lightBoxShader("lighting_box.vs", "lighting_box.fs");
	/*****************Build And Compile Our Shader Program Ends********************/

	/********Initialize some data starts************/
	//object positions
	gvObjectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
	gvObjectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
	gvObjectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
	gvObjectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
	gvObjectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
	gvObjectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
	gvObjectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
	gvObjectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
	gvObjectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));

	//lighting info
	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		//calculate slightly random offsets
		float xPos = ((rand() % 100) / 100.0f) * 6.0 - 3.0;
		float yPos = ((rand() % 100) / 100.0f) * 6.0 - 4.0;
		float zPos = ((rand() % 100) / 100.0f) * 6.0 - 3.0;
		gvLightPositions.push_back(glm::vec3(xPos, yPos, zPos));

		//calculate random color
		float rColor = ((rand() % 100) / 200.0f) + 0.5; //between 0.5 and 1.0
		float gColor = ((rand() % 100) / 200.0f) + 0.5; //between 0.5 and 1.0
		float bColor = ((rand() % 100) / 200.0f) + 0.5; //between 0.5 and 1.0
		gvLightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}
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
	
	lightingPassShader.use();
	lightingPassShader.setInt("u_sampler_gPositionCBO", 0);
	lightingPassShader.setInt("u_sampler_gNormalCBO", 1);
	lightingPassShader.setInt("u_sampler_gAlbedoSpecCBO", 2);
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
		GL_RGBA, //internal format
		WIN_WIDTH, //width
		WIN_HEIGHT, //height
		0, //border
		GL_RGBA, //image format
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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
											100.0f //far plane distance from near plane
											);

		//activate shader program object
		geometryPassShader.use();
		//pass values to uniform
		geometryPassShader.setMat4("u_view_matrix", gViewMatrix);
		geometryPassShader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);

			for(unsigned int i=0; i<gvObjectPositions.size(); i++)
			{
				gModelMatrix = glm::mat4(); //make identity matrix

				//model transformation
				glm::vec3 translate_object_vec = glm::vec3(0.0f, 0.0f, -3.0f);
				gModelMatrix = glm::translate(gModelMatrix,
					translate_object_vec //translate by
				);

				gModelMatrix = glm::translate(gModelMatrix,
					gvObjectPositions[i] //translate by
				);

				gModelMatrix = glm::scale(gModelMatrix,
					glm::vec3(0.25f) //scale by
				);

				geometryPassShader.setMat4("u_model_matrix", gModelMatrix);
				//draw
				ourModel.Draw(geometryPassShader);
			}
		
			//unbind framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			/******Geometry Pass Ends*********/

			/*
				2. Lighting Pass: calculate lighting by iterating over screen filled with quad pixel-by-pixel using the gbuffers contents
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

			//set light related uniforms
			for (unsigned int i = 0; i < gvLightPositions.size(); i++)
			{
				lightingPassShader.setVec3("u_lights["+std::to_string(i)+"].Position", gvLightPositions[i]);
				lightingPassShader.setVec3("u_lights[" + std::to_string(i) + "].Color", gvLightColors[i]);

				//update attenuation params
				const float constant = 1.0;
				const float linear = 0.7;
				const float quadratic = 1.8;
				lightingPassShader.setFloat("u_lights[" + std::to_string(i) + "].Linear", linear);
				lightingPassShader.setFloat("u_lights[" + std::to_string(i) + "].Quadratic", quadratic);
			}
			lightingPassShader.setVec3("u_viewer_position", camera.Position);
			//render quad
			glBindVertexArray(gQuadVAO); //bind necessary VAO
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0); //unbind VAO

			/*
				Copy contents of geometry's depth buffer to the default framebuffer
			*/
			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBufferFBO); //bind
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); //write to default framebuffer
			glBlitFramebuffer(
				0, //scrX0
				0, //scrY0
				WIN_WIDTH, //scrX1
				WIN_HEIGHT, //scrY1
				0, //dstX0
				0, //dstY0
				WIN_WIDTH, //dstX1
				WIN_HEIGHT, //dstY1
				GL_DEPTH_BUFFER_BIT, //mask
				GL_NEAREST //filter
			);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//3. render lights on top of the scene
			lightBoxShader.use();
			//pass values to uniform
			lightBoxShader.setMat4("u_view_matrix", gViewMatrix);
			lightBoxShader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);
			//set light related uniforms
			for (unsigned int i = 0; i < gvLightPositions.size(); i++)
			{
				gModelMatrix = glm::mat4(); //make identity matrix

				//model transformation
				glm::vec3 translate_object_vec = glm::vec3(0.0f, 0.0f, -3.0f);
				gModelMatrix = glm::translate(gModelMatrix,
					translate_object_vec //translate by
				);
				gModelMatrix = glm::translate(gModelMatrix,
					gvLightPositions[i] //translate by
				);

				gModelMatrix = glm::scale(gModelMatrix,
					glm::vec3(0.125f) //scale by
				);

				lightBoxShader.setMat4("u_model_matrix", gModelMatrix);
				lightBoxShader.setVec3("u_light_color", gvLightColors[i]);
				//render cube
				glBindVertexArray(gCubeVAO); //bind cube vao
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0); //unbind cube vao
			}
			
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