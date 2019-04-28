/*
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 23 Nov 2018

Program Name - a lighting scene - Shadow Mapping
Covers Basics of Shadow Mapping - output shows some shadow acne
Important Notes:
We can register the callback functions after we have created the window and before gameloop is initiated

The output of the vertex shader requires the coordinates to be in clip-space which is what we just did with
the transformation matrices. OpenGL then performs perspective division on the clip-space coordinates to transform
them to normalized-device coordinates. OpenGL then uses the parameters from glViewPort to map the normalized-device
coordinates to screen coordinates where each coordinate corresponds to a point on your screen (in our case a 800x600 screen).
This process is called the viewport transform.
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
unsigned int loadTexture(char const* path); //to load textures
void renderScene(const Shader &shader);
void renderCube();
void renderQuad();

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

										//set light position
glm::vec3 gLightPosition = glm::vec3(1.2f, 1.0f, -0.5f);

float deltaTime = 0.0f; //time between current frame and last frame
float lastFrame = 0.0f; //time of last frame

						//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f) //position
);
//mouse movements
bool firstMouse = true;
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;

//fullscreen
bool gbFullScreen = false;

//for blinn
bool isBPressed = false;

unsigned int VAO = 0, VBO = 0;
unsigned int VAO_Cube = 0;
unsigned int VBO_Cube = 0;
unsigned int VAO_Quad = 0;
unsigned int VBO_Quad = 0;

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
	int xPos = (mode->width / 2) - (WIN_WIDTH / 2);
	int yPos = (mode->height / 2) - (WIN_HEIGHT / 2);
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
	Shader shadow_rendering_shader("shadow_rendering.vs", "shadow_rendering.fs");
	Shader simple_depth_shader("simple_depth_shader.vs", "simple_depth_shader.fs");
	Shader debug_depth_quad("debug_vertex_shader.vs", "debug_fragment_shader.fs");

	shadow_rendering_shader.use();
	shadow_rendering_shader.setInt("u_sampler_diffuse_texture",0);
	shadow_rendering_shader.setInt("u_sampler_shadow_map",1);
	/*****************Build And Compile Our Shader Program Ends********************/

	//Input Vertex Data
	float vertices[] = {
		// positions            // normals         // texcoords
		10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};

	unsigned int woodTexture = loadTexture("resources/textures/wood.png");

	//create vertex array object
	//a VAO that stores our vertex attribute configuration
	//and which VBO to use
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
		(void *)0 //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(0); //enable vertex attribute; 0 is vertex attribute location

								  //normal data
	glVertexAttribPointer(
		1, //which vertex attribute we want to configure
		3, //size of the vertex attribute - 3 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		8 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *)(3 * sizeof(float)) //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(1); //enable vertex attribute; 0 is vertex attribute location

								  //tex coords data
	glVertexAttribPointer(
		2, //which vertex attribute we want to configure
		2, //size of the vertex attribute - 3 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		8 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *)(6 * sizeof(float)) //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(2);
	//this is allowed; the call to glVertexAttribPointer registerd VBO as the vertex attribute's bound vertex buffer object;
	//so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vertex buffer

									  //you can unbind the vao afterwards so that other VAO calls won't accidentaly modify this VAO
	glBindVertexArray(0);
	/*********Create Vertex Array Object for Cube ends********/

	/*******Create Framebuffer Starts*******/
	unsigned int depthMapFBO;
	//generate framebuffer
	glGenFramebuffers(1, &depthMapFBO); //this creates frame buffer in memory

										//create 2D texture that will be used as a framebuffer's depth buffer
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMap;
	glGenTextures(1, &depthMap); //this creates texture buffer in memory

								 //bind this texture to 2D texture target
	glBindTexture(GL_TEXTURE_2D, //texture
		depthMap //texture to bind
	);

	//allocate dimentions to the above created texture
	glTexImage2D(
		GL_TEXTURE_2D, //target
		0, //mipmap level
		GL_DEPTH_COMPONENT, //texture will be stored in this format because we only care about depth values
		SHADOW_WIDTH, //width
		SHADOW_HEIGHT, //height
		0, //border size
		GL_DEPTH_COMPONENT, //input format to this texture
		GL_FLOAT, //values data types
		NULL //data we are passing as null as of now, so that we can store it once we start to render the scene
	);

	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //when downscaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //when upscaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //s texcoord
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //t texcoord

																  //attach this generated texture to the framebuffer as a depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, //target
		GL_DEPTH_ATTACHMENT, //attach texture as a depth buffer because we only need depth information while rendering the scene from light's perspective
		GL_TEXTURE_2D, //the texture is of type GL_TEXTURE_2D
		depthMap, //actual texture
		0 //mipmap level
	);

	//framebuffer object is not complete without color buffer so we need to explicitly tell OpenGL
	//that we are not going to render any color data
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind the framebuffer
										  /*******Create Framebuffer Ends*******/

										  /*******Initialize OpenGL States**********/
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

		//Local Variables
		//load transformation matrices to identity matrix
		gModelMatrix = glm::mat4();
		gViewMatrix = glm::mat4();
		gPerspectiveProjectionMatrix = glm::mat4();
		glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

		//calculate new delta time value within each frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// rendering commands here
		// -----------------------

		//at the start of each iteration, clear the screen with color of your choice
		//else we should see the results from previous iteration
		//entire color buffer will be filled with this color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		/*************First Render To DepthMap Starts*******************/

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

		simple_depth_shader.use();

		//configure shader and matrices from lights point of view
		float near_plane = 1.0f;
		float far_plane = 7.5f;
		glm::mat4 light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		glm::mat4 light_view = glm::lookAt(
			lightPos, //eye
			glm::vec3(0.0f, 0.0f, 0.0f), //looking at
			glm::vec3(0.0f, 1.0f, 0.0f) //up vector
		);

		glm::mat4 light_Space_Matrix = light_projection * light_view;

		//render scene from lights point of view

		//set uniforms
		simple_depth_shader.setMat4("u_light_space_matrix", light_Space_Matrix);

		//bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		//clear depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);

		//bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		renderScene(simple_depth_shader);

		//unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		/*************First Render To DepthMap Ends*******************/

		/*****Render Scene As Normal: Starts**********/
		//reset viewport
		glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
		//clear the screens color buffer
		//here we pass in buffer bits to specify which buffer we would like to clear
		glClear(GL_COLOR_BUFFER_BIT //clear color buffer
			| GL_DEPTH_BUFFER_BIT //clear depth buffer
		);

		gViewMatrix = camera.GetViewMatrix(); //View Transformation
		//perspective transformation
		gPerspectiveProjectionMatrix = glm::perspective(
			glm::radians(camera.Zoom), //fovy
			(float)WIN_WIDTH / (float)WIN_HEIGHT, //aspect ratio
			0.1f, //near plane distance from camera
			100.0f //far plane distance from near plane
		);
		shadow_rendering_shader.use();

		//pass uniforms
		shadow_rendering_shader.setMat4("u_view_matrix", gViewMatrix);
		shadow_rendering_shader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);
		shadow_rendering_shader.setVec3("u_light_position", lightPos);
		shadow_rendering_shader.setVec3("u_viewer_position", camera.Position);
		shadow_rendering_shader.setMat4("u_light_space_matrix", light_Space_Matrix);

		//activate textures
		glActiveTexture(GL_TEXTURE0); //activate texture unit 0
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1); //activate texture unit 1
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderScene(shadow_rendering_shader);
		/*****Render Scene As Normal: Ends**********/

		/******Render Depthmap to Quad For Visual Debugging: Starts***********/
		debug_depth_quad.use();
		debug_depth_quad.setInt("u_depthMap", 0);
		debug_depth_quad.setFloat("near_plane", near_plane);
		debug_depth_quad.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//renderQuad();
		/******Render Depthmap to Quad For Visual Debugging: Ends***********/
		//swap color buffers - a large buffer that contains the color values for each pixel's in GLFW window
		//that has been used to draw in during this iteration and show it as the output to the screen
		glfwSwapBuffers(window);

		//check if any events are triggered
		glfwPollEvents();
	}

	//glfw: clean/delete all GLFW's resources that were allocated
	if (VAO)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	if (VBO)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO_Cube)
	{
		glDeleteVertexArrays(1, &VAO_Cube);
		VAO_Cube = 0;
	}
	if (VBO_Cube)
	{
		glDeleteBuffers(1, &VBO_Cube);
		VBO_Cube = 0;
	}
	if (VAO_Quad)
	{
		glDeleteVertexArrays(1, &VAO_Quad);
		VAO_Quad = 0;
	}
	if (VBO_Quad)
	{
		glDeleteBuffers(1, &VBO_Quad);
		VBO_Quad = 0;
	}
	glfwTerminate();

	return 0;
}

/**
renderScene: Renders 3D scene
*/
void renderScene(const Shader &shader)
{
	glm::mat4 model;
	shader.setMat4("u_model_matrix", model); //set model uniform
	glBindVertexArray(VAO);

	//draw floor
	glDrawArrays(
		GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
		0, //starting index of enabled vertex arrays
		6 //how many vertices you want to draw
	);

	glBindVertexArray(0); //unbind the VAO

						  //cubes
						  //cube 1:
	model = glm::mat4(); //set to identity

						 //model transformation
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("u_model_matrix", model); //set model uniform
	renderCube();

	//cube 2:
	model = glm::mat4(); //set to identity

						 //model transformation
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("u_model_matrix", model); //set model uniform
	renderCube();


	//cube 3:
	model = glm::mat4(); //set to identity

						 //model transformation
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
	model = glm::scale(model, glm::vec3(0.25f));
	shader.setMat4("u_model_matrix", model); //set model uniform
	renderCube();

}

/*
renderCube: Draw 3D cube
*/
void renderCube()
{
	//if VAO is not yet created
	if (VAO_Cube == 0)
	{
		/******Initialization Data: Starts******/
		float cubeVertices[] = {
			//position			//normals			//tex coords
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
		/******Initialization Data: Ends******/

		/****Cube VAO Creation: Starts******/
		glGenVertexArrays(1, &VAO_Cube); //generates VAO in memory
		glBindVertexArray(VAO_Cube); //bind this VAO to the target

									 /****Cube VAO::VBO Creation: Starts******/
		glGenBuffers(1, &VBO_Cube); //generates buffer in memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube); //bind VBO to the target
												 //add data to the VBO
		glBufferData(
			GL_ARRAY_BUFFER, //target
			sizeof(cubeVertices), //size of data
			cubeVertices, //actual data
			GL_STATIC_DRAW //data is not changing so frequently
		);

		//tell OpenGL how to use this data
		//position data
		glVertexAttribPointer(
			0, //attribute location in vertex shader
			3, //size of 0th attribute: 0th attribute is of 3 floats
			GL_FLOAT, //data type of attribute data
			GL_FALSE, //no need of normalization of data
			8 * sizeof(float), //stride
			(void *)0 //offset of 0th attribute in the buffer
		);

		glEnableVertexAttribArray(0); //enable 0th attribute

									  //normal data
		glVertexAttribPointer(
			1, //attribute location in vertex shader
			3, //size of 0th attribute: 0th attribute is of 3 floats
			GL_FLOAT, //data type of attribute data
			GL_FALSE, //no need of normalization of data
			8 * sizeof(float), //stride
			(void *)(3 * sizeof(float)) //offset of 1st attribute in the buffer
		);

		glEnableVertexAttribArray(1); //enable 0th attribute

									  //tex coords data
		glVertexAttribPointer(
			2, //attribute location in vertex shader
			2, //size of 0th attribute: 2nd attribute is of 3 floats
			GL_FLOAT, //data type of attribute data
			GL_FALSE, //no need of normalization of data
			8 * sizeof(float), //stride
			(void *)(6 * sizeof(float)) //offset of 0th attribute in the buffer
		);

		glEnableVertexAttribArray(2); //enable 0th attribute

		glBindBuffer(GL_VERTEX_ARRAY, 0); //unbind buffer
										  /****Cube VAO::VBO Creation: Ends******/

		glBindVertexArray(0); //unbind VAO
							  /****Cube VAO Creation: Ends******/
	}
	//bind Cube Vao
	glBindVertexArray(VAO_Cube);

	//draw cube
	glDrawArrays(
		GL_TRIANGLES, //drawing primitive
		0, //first vertex in the buffer
		36 //36 vertices to draw
	);
	//unbind Cube Vao
	glBindVertexArray(0);
}

/**
renderQuad: Render 2D Quad
*/
void renderQuad()
{
	//if VAO is not yet created
	if (VAO_Quad == 0)
	{
		/******Initialization Data: Starts******/
		float vertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		/******Initialization Data: Ends******/

		/****Cube VAO Creation: Starts******/
		glGenVertexArrays(1, &VAO_Quad); //generates VAO in memory
		glBindVertexArray(VAO_Quad); //bind this VAO to the target

									 /****Cube VAO::VBO Creation: Starts******/
		glGenBuffers(1, &VBO_Quad); //generates buffer in memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Quad); //bind VBO to the target
												 //add data to the VBO
		glBufferData(
			GL_ARRAY_BUFFER, //target
			sizeof(vertices), //size of data
			vertices, //actual data
			GL_STATIC_DRAW //data is not changing so frequently
		);

		//tell OpenGL how to use this data
		//position data
		glVertexAttribPointer(
			0, //attribute location in vertex shader
			3, //size of 0th attribute: 0th attribute is of 3 floats
			GL_FLOAT, //data type of attribute data
			GL_FALSE, //no need of normalization of data
			5 * sizeof(float), //stride
			(void *)0 //offset of 0th attribute in the buffer
		);

		glEnableVertexAttribArray(0); //enable 0th attribute

									  //tex coords data
		glVertexAttribPointer(
			1, //attribute location in vertex shader
			2, //size of 0th attribute: 1nd attribute is of 2 floats
			GL_FLOAT, //data type of attribute data
			GL_FALSE, //no need of normalization of data
			5 * sizeof(float), //stride
			(void *)(3 * sizeof(float)) //offset of 0th attribute in the buffer
		);

		glEnableVertexAttribArray(1); //enable 1th attribute

		glBindBuffer(GL_VERTEX_ARRAY, 0); //unbind buffer
										  /****Cube VAO::VBO Creation: Ends******/

		glBindVertexArray(0); //unbind VAO
							  /****Cube VAO Creation: Ends******/
	}
	//bind Cube Vao
	glBindVertexArray(VAO_Quad);

	//draw cube
	glDrawArrays(
		GL_TRIANGLE_STRIP, //drawing primitive
		0, //first vertex in the buffer
		4 //4 vertices to draw
	);
	//unbind Cube Vao
	glBindVertexArray(0);
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
	//B
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		if (isBPressed == true)
		{
			isBPressed = false;
		}
		else
		{
			isBPressed = true;
		}
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

/*loadTexture: Utility function for loading a 2D Texture from a file*/
unsigned int loadTexture(char const* path)
{
	//texture ID
	unsigned int textureID;
	//generate texture
	glGenTextures(
		1, //how many texture objects you want to generate
		&textureID //it will contain id of the texture generated over GPU
	);


	//LOAD IMAGE, CREATE TEXTURES AND GENERATE MIPMAPS
	int width, height, nrChannels;

	//load image data
	unsigned char* data = stbi_load(
		path, //file path
		&width, //this will contain width of the image
		&height, //this will contain height of the image
		&nrChannels, //this will contain number of colored channels
		0 //req_comp
	);

	//check if image data successfully loaded
	if (data) //if yes
	{
		//format of the image
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		//bind the above texture to the Texture type
		//all the upcoming GL_TEXTURE_2D operations will now affect this texture object
		glBindTexture(GL_TEXTURE_2D, //binding to this texture type
			textureID //texture to bind
		);

		//generate texture from this texture data
		glTexImage2D(
			GL_TEXTURE_2D, //target; this operation will generate texture on currently bound texture object
			0, //base mipmap level
			format, //internal format; in what kind of format you want to store the texture
			width, //width of resulting texture
			height, //height of resulting texture
			0, //this should be always 0 for legacy purpose
			format, //format of source image; png has Alpha value also
			GL_UNSIGNED_BYTE, //data is in GL_UNSIGNED_BYTE format
			data //actual data
		);

		//now the currently bound texture object has the texture image attached to it
		//but it only has the base-level of the texture image loaded
		//below will automatically generate all the required mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);

		//set texture wrapping parameters
		glTexParameteri(
			GL_TEXTURE_2D, //target type
			GL_TEXTURE_WRAP_S, //wrap for s texture axis
			GL_REPEAT //this is the default value
		);

		glTexParameteri(
			GL_TEXTURE_2D, //target type
			GL_TEXTURE_WRAP_T, //wrap for t texture axis
			GL_REPEAT //this is the default value
		);

		//set texture filtering parameters
		glTexParameteri(
			GL_TEXTURE_2D, //target type
			GL_TEXTURE_MIN_FILTER, //when scaling down
			GL_LINEAR //takens interpolated value from the tex coordinates neighbouring texels
		);

		glTexParameteri(
			GL_TEXTURE_2D, //target type
			GL_TEXTURE_MAG_FILTER, //when scaling up
			GL_LINEAR //takens interpolated value from the tex coordinates neighbouring texels
		);

	}
	else //if no
	{
		std::cout << "ERROR::TEXTURE::LOADING_IMAGE_FAILED" << std::endl;
	}

	//free image data now
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

	return textureID;
}