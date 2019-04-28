/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 26 December 2018

	Program Name - Bloom - not working properly - check once again
	To implement bloom we render a lighted scene as usual and extract both the scene's HDR colorbuffer and an image of the
	scene with only its bright regions visible. The extracted brightness image is then blurred and the result added on top
	of the original HDR scene image.

	Exposure balance using Q and E keys
	HDR toggle using space bar

	Important Notes:
	We can register the callback functions after we have created the window and before gameloop is initiated

	The output of the vertex shader requires the coordinates to be in clip-space which is what we just did with 
	the transformation matrices. OpenGL then performs perspective division on the clip-space coordinates to transform 
	them to normalized-device coordinates. OpenGL then uses the parameters from glViewPort to map the normalized-device 
	coordinates to screen coordinates where each coordinate corresponds to a point on your screen (in our case a 800x600 screen). 
	This process is called the viewport transform.

	OpenGL reads texture coordinates with the y or v coordinates reversed from how textures are generally created.
*/

//header files
#include<iostream>
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

//by defining this, the preprocessor modifies the header file such that it contains only the relevant definition source code
#define STB_IMAGE_IMPLEMENTATION
#include "Include\stb_image.h"

//global function declarations
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path, bool gammaCorrection);

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
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f) //position
				);
//mouse movements
bool firstMouse = true;
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;

//fullscreen
bool gbFullScreen = false;

//lighting-info
std::vector<glm::vec3> gLightPositions; //positions
std::vector<glm::vec3> gLightColors; //colors

float gfExposure = 1.0f;
bool gbBloom = true;
bool gbBloomKeyPressed = false;

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
									"OpenGL Window", //title
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
	Shader shader("vertex_shader.vs", "fragment_shader.fs");
	Shader lightShader("vertex_shader.vs", "light_box.fs");
	Shader blurShader("blur.vs", "blur.fs");
	Shader bloomFinalShader("bloom_final.vs", "bloom_final.fs");
	/*****************Build And Compile Our Shader Program Ends********************/

	/********Initialize data starts***************/
	//Input Vertex Data
	float vertices[] = {
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

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	//load texture data
	unsigned int woodTexture = loadTexture(
		"resources/textures/wood.png", //path of the image
		true //gamma correction needed
		);

	unsigned int containerTexture = loadTexture(
		"resources/textures/container2.png", //path of the image
		true //gamma correction needed
	);

	//lighting positions
	gLightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	gLightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	gLightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	gLightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
	//lighting colors
	gLightColors.push_back(glm::vec3(2.0f, 2.0f, 2.0f));
	gLightColors.push_back(glm::vec3(1.5f, 0.0f, 0.0f));
	gLightColors.push_back(glm::vec3(0.0f, 0.0f, 1.5f));
	gLightColors.push_back(glm::vec3(0.0f, 1.5f, 0.0f));

	shader.use();
	shader.setInt("u_sampler_diffuse_texture", 0);

	blurShader.use();
	blurShader.setInt("u_sampler_image", 0);

	bloomFinalShader.use();
	bloomFinalShader.setInt("u_sampler_hdr_scene", 0);
	bloomFinalShader.setInt("u_sampler_bloom_blur", 1);

	/********Initialize data ends***************/

	/*********Framebuffers, VAO, VBO Initialization starts********************/
	//configure floating point framebuffer
	unsigned int FBO_hdr;
	glGenFramebuffers(1, &FBO_hdr);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_hdr);

	//create 2 floating point color buffers - one for normal rendering and other for brightness threshold value
	unsigned int colorBuffer[2];
	glGenTextures(2, colorBuffer);

	for(int i=0; i<2; i++)
	{
	glBindTexture(GL_TEXTURE_2D, colorBuffer[i]);
	glTexImage2D(
		GL_TEXTURE_2D, //target
		0, //mipmap level
		GL_RGBA16F, //16-bit color internal format
		WIN_WIDTH, //width
		WIN_HEIGHT, //height
		0, //border
		GL_RGB, //image format
		GL_FLOAT, //data type
		NULL //data is not present now
	);
	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffer[i], 0);
	}
	//create depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIN_WIDTH, WIN_HEIGHT);

	//attach buffers
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	//tell OpenGL which color attachment we'll use for rendering
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	//check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not complete!" << std::endl;
	}

	
	//unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//ping-pong framebuffers for blurring
	unsigned int pingpongFBO[2];
	unsigned int pingpongCBO[2];

	//generate framebuffers
	glGenFramebuffers(2, pingpongFBO);

	//create textures for color buffers
	glGenTextures(2, pingpongCBO);

	for (unsigned int i = 0; i < 2; i++)
	{
		//bind framebuffers
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);

		//bind textures
		glBindTexture(GL_TEXTURE_2D, pingpongCBO[i]);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB16F, //internal format
			WIN_WIDTH,
			WIN_HEIGHT,
			0,
			GL_RGB, //image format
			GL_FLOAT, //type
			NULL //data
		);
	

		//set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//attach color buffers
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, //target
			GL_COLOR_ATTACHMENT0, //attachment
			GL_TEXTURE_2D,
			pingpongCBO[i],
			0 //level
		);

		//check if framebuffers are complete - no need of depth buffer
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR:: Framebuffer not complete" << std::endl;
		}

	}
	//create vertex array object
	//a VAO that stores our vertex attribute configuration
	//and which VBO to use
	unsigned int VAO, VBO;
	glGenVertexArrays(1,  //how many vertex array objects you want to create
					&VAO //id of vertex array object is store in this
				);
	//bind vertex array object first, then bind and set vertex buffers
	//and then configure vertex attributes
	glBindVertexArray(VAO);

	//create memory on GPU - vertex buffer object
	glGenBuffers(
			1, //how many buffers you want to create
			&VBO //this will contain the buffer id of the created buffer on GPU
			);

	//here onwards, any buffer call(on GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer, VBO
	glBindBuffer(
			GL_ARRAY_BUFFER, //type of created buffer on GPU
			VBO //created buffer
			);

	//copy previously defined data to the buffer's memory
	glBufferData(
				GL_ARRAY_BUFFER, //this type of buffer is your target, which is VBO
				sizeof(vertices), //size of the data which we will pass
				vertices, //actual data
				GL_STATIC_DRAW //specify how you want graphics card will manage the data in the buffer
			);

	
	//tell OpenGL that how to link vertex data to vertex attributes in shader
	//position data
	glVertexAttribPointer(
					0, //which vertex attribute we want to configure
					3, //size of the vertex attribute - 3 float values for each vertex
					GL_FLOAT, //type of the data
					GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
					8 * sizeof(float), //space between consecutive vertex attribtes - stride
					(void *) 0 //from where the position data begins in the buffer
				);

	glEnableVertexAttribArray(0); //enable vertex attribute; 0 is vertex attribute location

	//normal data
	glVertexAttribPointer(
		1, //which vertex attribute we want to configure
		3, //size of the vertex attribute - 3 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		8 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *) (3 * sizeof(float)) //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(1); //enable vertex attribute; 1 is vertex attribute location

	//tex coord data
	glVertexAttribPointer(
		2, //which vertex attribute we want to configure
		2, //size of the vertex attribute - 2 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		8 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *)(6 * sizeof(float)) //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(2); //enable vertex attribute; 02is vertex attribute location

	//this is allowed; the call to glVertexAttribPointer registerd VBO as the vertex attribute's bound vertex buffer object;
	//so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vertex buffer

	//you can unbind the vao afterwards so that other VAO calls won't accidentaly modify this VAO
	glBindVertexArray(0);
	/*********Create Vertex Array Object for Cube ends********/

	/*****CREATE VERTEX ARRAY OBJECT FOR QUAD START*******/
	unsigned int vao_quad;
	unsigned int vbo_quad;

	glGenVertexArrays(1, //how many vertex array objects 
		&vao_quad //this will contain id of the vao created on GPU
		);
		
	//bind vertex array object
	glBindVertexArray(vao_quad);

	glGenBuffers(1, &vbo_quad);
	//buffer objects can be of many types, but I want this buffer object to bound to Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, //type of buffer you want to bind 
		vbo_quad //bind this vbo
				);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	//tell how this data is organized
	//position
	glVertexAttribPointer(0, //0th layout location attribute in the shader
		3, //3 float values for thsi attribute
		GL_FLOAT, //type of the values
		GL_FALSE, //normalization not needed
		5 * sizeof(float), //space between consecutive vertex attributes - stride
		(void *) 0 //from where position data begins in the buffer
		);

	//enable the 0th attribute
	glEnableVertexAttribArray(0);
	
	//tex coords
	glVertexAttribPointer(1, //0th layout location attribute in the shader
		2, //2 float values for thsi attribute
		GL_FLOAT, //type of the values
		GL_FALSE, //normalization not needed
		5 * sizeof(float), //space between consecutive vertex attributes - stride
		(void *) (3 * sizeof(float)) //from where position data begins in the buffer
	);

	//enable the 0th attribute
	glEnableVertexAttribArray(1);

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, //type of buffer you want to unbind 
		0 //0 means unbinding the buffer
	);

	//unbind vertex array object
	glBindVertexArray(0); //0 means unbinding the vao

	/*****CREATE VERTEX ARRAY OBJECT FOR LAMP ENDS*******/
	/*********Framebuffers, VAO, VBO Initialization ends********************/

	/*******Initialize OpenGL states**********/
	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	//tell OpenGL the size of the rendering window
	glViewport(0, 0,  //location of the lower left corner of the window
			VIEWPORT_WIDTH, VIEWPORT_HEIGHT //width and height of the rendering window
		);

	//register framebuffer_size_callback for adjusting viewport according to the window resizing
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// render loop
	// ------------
	while (!glfwWindowShouldClose(window)) //check if GLFW has been instructed to close
	{
		// input
		//-------
		processInput(window);

		// rendering commands here
		// -----------------------
		//at the start of each iteration, clear the screen with color of your choice
		//else we should see the results from previous iteration
		//entire color buffer will be filled with this color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//1. render scene into floating point framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_hdr);


		//clear the screens color buffer
		//here we pass in buffer bits to specify which buffer we would like to clear
		glClear(GL_COLOR_BUFFER_BIT //clear color buffer
				| GL_DEPTH_BUFFER_BIT //clear depth buffer
				);


		//draw our first triangle
		//activate shader program object
		shader.use();
		
		/**********CREATE TRANSFORMATION MATRICES***************/
		//load transformation matrices to identity matrix
		gModelMatrix = glm::mat4();
		gViewMatrix = glm::mat4();
		gPerspectiveProjectionMatrix = glm::mat4();
		//model transformation is done in for loop below

		//view transformation
		/*
		//we are translating the scene in the reverse direction of where we want to move
		gViewMatrix = glm::translate(gViewMatrix, //view matrix
								glm::vec3(0.0f, 0.0f, -3.0f) //translate scene towards -ve z axis
								);
		*/
		//calculate new delta time value within each frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		gViewMatrix = camera.GetViewMatrix();

		//perspective transformation
		gPerspectiveProjectionMatrix = glm::perspective(
											glm::radians(camera.Zoom), //fovy
											(float)WIN_WIDTH/(float)WIN_HEIGHT, //aspect ratio
											0.1f, //near plane distance from camera
											100.0f //far plane distance from near plane
											);

		//pass uniforms
		shader.setMat4("u_view_matrix", gViewMatrix);
		shader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);

		//bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);

		//set lighting uniforms
		for (unsigned int i = 0; i < gLightPositions.size(); i++)
		{
			shader.setVec3("lights["+std::to_string(i)+"].Position", gLightPositions[i]);
			shader.setVec3("lights[" + std::to_string(i) + "].Color", gLightColors[i]);
		}

		shader.setVec3("u_viewer_position", camera.Position);
		
		//render tunnel
		//bind which VAO we are going to use with the program object
		glBindVertexArray(VAO);

		//draw cubes with different model matrices
		//create one large cube that acts as a floor
			gModelMatrix = glm::mat4(); //set to identity
			gModelMatrix = glm::translate(gModelMatrix, glm::vec3(0.0f, -1.0f, 0.0)); //translate
			gModelMatrix = glm::scale(gModelMatrix, glm::vec3(12.5f, 0.5f, 12.5f)); //scale
			
			shader.setMat4("u_model_matrix", gModelMatrix);
			shader.setInt("u_inverse_normals", true);
			//draw cube
			glDrawArrays(
				GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
				0, //starting index of enabled vertex arrays
				36 //how many vertices you want to draw
			);
		
		
		//create multiple cubes as a scenery
		//bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerTexture);
		

		//cube 1
		gModelMatrix = glm::mat4(); //set to identity
		gModelMatrix = glm::translate(gModelMatrix, glm::vec3(0.0f, 1.5f, 0.0)); //translate
		gModelMatrix = glm::scale(gModelMatrix, glm::vec3(0.5f)); //scale

		shader.setMat4("u_model_matrix", gModelMatrix);
		shader.setInt("u_inverse_normals", true);
		//draw cube
		glDrawArrays(
			GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
			0, //starting index of enabled vertex arrays
			36 //how many vertices you want to draw
		);

		//cube 2
		gModelMatrix = glm::mat4(); //set to identity
		gModelMatrix = glm::translate(gModelMatrix, glm::vec3(2.0f, 0.0f, 1.0)); //translate
		gModelMatrix = glm::scale(gModelMatrix, glm::vec3(0.5f)); //scale

		shader.setMat4("u_model_matrix", gModelMatrix);
		shader.setInt("u_inverse_normals", true);
		//draw cube
		glDrawArrays(
			GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
			0, //starting index of enabled vertex arrays
			36 //how many vertices you want to draw
		);

		//cube 3
		gModelMatrix = glm::mat4(); //set to identity
		gModelMatrix = glm::translate(gModelMatrix, glm::vec3(-1.0f, -1.0f, 2.0)); //translate
		gModelMatrix = glm::rotate(gModelMatrix, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); //rotate

		shader.setMat4("u_model_matrix", gModelMatrix);
		shader.setInt("u_inverse_normals", true);
		//draw cube
		glDrawArrays(
			GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
			0, //starting index of enabled vertex arrays
			36 //how many vertices you want to draw
		);

		//cube 4
		gModelMatrix = glm::mat4(); //set to identity
		gModelMatrix = glm::translate(gModelMatrix, glm::vec3(0.0f, 2.7f, 4.0)); //translate
		gModelMatrix = glm::rotate(gModelMatrix, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); //rotate
		gModelMatrix = glm::scale(gModelMatrix, glm::vec3(1.25f)); //scale
		shader.setMat4("u_model_matrix", gModelMatrix);
		shader.setInt("u_inverse_normals", true);
		//draw cube
		glDrawArrays(
			GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
			0, //starting index of enabled vertex arrays
			36 //how many vertices you want to draw
		);

		//cube 5
		gModelMatrix = glm::mat4(); //set to identity
		gModelMatrix = glm::translate(gModelMatrix, glm::vec3(-2.0f, 1.0f, -3.0)); //translate
		gModelMatrix = glm::rotate(gModelMatrix, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); //rotate
		shader.setMat4("u_model_matrix", gModelMatrix);
		shader.setInt("u_inverse_normals", true);
		//draw cube
		glDrawArrays(
			GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
			0, //starting index of enabled vertex arrays
			36 //how many vertices you want to draw
		);


		//cube 6
		gModelMatrix = glm::mat4(); //set to identity
		gModelMatrix = glm::translate(gModelMatrix, glm::vec3(-3.0f, 0.0f, 0.0)); //translate
		gModelMatrix = glm::scale(gModelMatrix, glm::vec3(0.5f)); //scale
		shader.setMat4("u_model_matrix", gModelMatrix);
		shader.setInt("u_inverse_normals", true);
		//draw cube
		glDrawArrays(
			GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
			0, //starting index of enabled vertex arrays
			36 //how many vertices you want to draw
		);

		glBindVertexArray(0); //unbind the VAO


		//2. Now show all the light sources as a bright cubes
		lightShader.use();
		//pass uniforms
		lightShader.setMat4("u_view_matrix", gViewMatrix);
		lightShader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);

		//bind which VAO we are going to use with the program object
		glBindVertexArray(VAO);

		for (int i = 0; i < gLightPositions.size(); i++)
		{
			gModelMatrix = glm::mat4(); //set to identity
			gModelMatrix = glm::translate(gModelMatrix, gLightPositions[i]); //translate
			gModelMatrix = glm::scale(gModelMatrix, glm::vec3(0.25f)); //scale
			lightShader.setMat4("u_model_matrix", gModelMatrix);
			lightShader.setVec3("u_light_color", gLightColors[i]);
			lightShader.setInt("u_inverse_normals", true);

			//draw cube
			glDrawArrays(
				GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
				0, //starting index of enabled vertex arrays
				36 //how many vertices you want to draw
			);
		}

		//unbind VAO
		glBindVertexArray(0);

		//unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//3. blur bright fragments with two-pass Gaussian Blur
		bool horizontal = true;
		bool first_iteration = true;
		unsigned int amount = 10;
		blurShader.use();

		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			blurShader.setInt("u_horizontal", horizontal);

			//bind texture of other framebuffer (or scene if first iteration)
			glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffer[1] : pingpongCBO[!horizontal]);

			//render quad
			//bind vao
			glBindVertexArray(vao_quad);
			//draw cube
			glDrawArrays(
				GL_TRIANGLE_STRIP, //what we want to draw using the vertices given in the shader program object
				0, //starting index of enabled vertex arrays
				4 //how many vertices you want to draw
			);
			glBindVertexArray(0); //unbind the VAO

			horizontal = !horizontal;
			if (first_iteration)
			{
				first_iteration = false;
			}
		}
		//unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//4. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffers (clamped) color range
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		bloomFinalShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffer[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongCBO[!horizontal]);
		bloomFinalShader.setInt("u_bloom", gbBloom);
		bloomFinalShader.setFloat("u_exposure", gfExposure);
		//bind vao
		glBindVertexArray(vao_quad);
		//draw cube
		glDrawArrays(
			GL_TRIANGLE_STRIP, //what we want to draw using the vertices given in the shader program object
			0, //starting index of enabled vertex arrays
			4 //how many vertices you want to draw
		);
		glBindVertexArray(0); //unbind the VAO
		/*********Draw LAMP END**************/
		
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
	//hdr
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !gbBloomKeyPressed)
	{
		gbBloom = !gbBloom;
		gbBloomKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		gbBloomKeyPressed = false;
	}

	//adjust exposure on Q key press
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		if (gfExposure > 0.0f)
		{
			gfExposure -= 0.1f;
		}
		else
		{
			gfExposure = 0.0f;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		gfExposure += 0.1f;
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

unsigned int loadTexture(char const* path, bool gammaCorrection)
{
	unsigned int textureID; //id of the texture

	//generate texture
	glGenTextures(1, &textureID); //creates a texture and assigns its id to textureID

	//load texture data
	int width, height, nrComponents;
	unsigned char *data = stbi_load(
		path, //file name
		&width, //width of a texture
		&height, //height of a texture
		&nrComponents, //color components
		0 //req_comp
	);

	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;

		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		} else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		//define texture attributes
		//bind texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(
			GL_TEXTURE_2D, //target
			0, //mipmap level
			internalFormat, //internal format of a texture
			width, //width
			height, //height
			0, //border
			dataFormat , //image format
			GL_UNSIGNED_BYTE, //type of image data
			data //actual data
		);

		//set texture params
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //scrolling down
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scrolling up

		//generate mipmaps for image
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}

	//free image data
	stbi_image_free(data);

	return textureID;
}