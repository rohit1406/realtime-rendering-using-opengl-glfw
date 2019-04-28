/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 10 Nov 2018

	Program Name - Model Loading using Assimp - Open Asset Import Library 
	Planet Around Asteroid using instanced rendering

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
Camera camera(glm::vec3(0.0f, 0.0f, 55.0f) //position
				);
//mouse movements
bool firstMouse = true;
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;

//fullscreen
bool gbFullScreen = false;
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
	Shader ourPlanetShader("vertex_shader_planet.vs", "fragment_shader_planet.fs");
	Shader ourShader("vertex_shader.vs", "fragment_shader.fs");
	/*****************Build And Compile Our Shader Program Ends********************/
	cout << "Shaders Loaded" << endl;
	//load model
	cout << "Model Loading started" << endl;
	//Model ourModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
	Model ourPlanet("resources/objects/planet/planet.obj");
	Model ourAsteroid("resources/objects/rock/rock.obj");
	//Model ourModel("Cube.dae");
	cout << "Model Loaded" << endl;

	/************Data Initialization Starts********************/
	unsigned int amount = 55000;
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); //initialize random seed
	float radius = 50.0f;
	float offset = 2.5f;

	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		//1. translation: displace along circle with radius in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; //keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		//2. scale: scale between 0.05f and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		//3. rotation: add random rotation around a semi randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		//4. now add to list of matrices
		modelMatrices[i] = model;
	}
	/************Data Initialization Ends********************/

	/**********VAO Configuration Starts**************/
	/*****VBO configuration starts********/
	unsigned int buffer;

	//generate buffer
	glGenBuffers(1, &buffer);

	//bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	//pass data to buffer
	glBufferData(GL_ARRAY_BUFFER, //target
		amount * sizeof(glm::mat4), //size of data
		&modelMatrices[0], //actual data
		GL_STATIC_DRAW
		);

	for (unsigned int i = 0; i < ourAsteroid.meshes.size(); i++)
	{
		unsigned int VAO = ourAsteroid.meshes[i].VAO;
		glBindVertexArray(VAO);

		//vertex attributes
		GLsizei vec4Size = sizeof(glm::vec4);
		glVertexAttribPointer(
			5, //attribute intex
			4, //each attribute will have 4 floats
			GL_FLOAT, //data type of attribute values
			GL_FALSE, //normalization of values not required
			4 * vec4Size, //size of data
			(void *) 0 //offset
		);
		glEnableVertexAttribArray(5);

		glVertexAttribPointer(
			6, //attribute intex
			4, //each attribute will have 4 floats
			GL_FLOAT, //data type of attribute values
			GL_FALSE, //normalization of values not required
			4 * vec4Size, //size of data
			(void *) (vec4Size) //offset
		);
		glEnableVertexAttribArray(6);

		glVertexAttribPointer(
			7, //attribute intex
			4, //each attribute will have 4 floats
			GL_FLOAT, //data type of attribute values
			GL_FALSE, //normalization of values not required
			4 * vec4Size, //size of data
			(void *)(2 * vec4Size)//offset
		);
		glEnableVertexAttribArray(7);

		glVertexAttribPointer(
			8, //attribute intex
			4, //each attribute will have 4 floats
			GL_FLOAT, //data type of attribute values
			GL_FALSE, //normalization of values not required
			4 * vec4Size, //size of data
			(void *)(3 * vec4Size) //offset
		);
		glEnableVertexAttribArray(8);

		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);

		glBindVertexArray(0);
	}
	/*****VBO configuration ends********/
	/**********VAO Configuration Ends**************/
	/*******Initialize some params**********/
	//enable depth testing
	glEnable(GL_DEPTH_TEST);
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

		// rendering commands here
		// -----------------------
		//at the start of each iteration, clear the screen with color of your choice
		//else we should see the results from previous iteration
		//entire color buffer will be filled with this color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//clear the screens color buffer
		//here we pass in buffer bits to specify which buffer we would like to clear
		glClear(GL_COLOR_BUFFER_BIT //clear color buffer
				| GL_DEPTH_BUFFER_BIT //clear depth buffer
				);


		//draw our first triangle
		//activate shader program object
		ourPlanetShader.use();
		
		/**********CREATE TRANSFORMATION MATRICES***************/
		//load transformation matrices to identity matrix
		gModelMatrix = glm::mat4();
		gViewMatrix = glm::mat4();
		gPerspectiveProjectionMatrix = glm::mat4();
		
		//calculate new delta time value within each frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//view transformation
		gViewMatrix = camera.GetViewMatrix();

		//perspective transformation
		gPerspectiveProjectionMatrix = glm::perspective(
											glm::radians(camera.Zoom), //fovy
											(float)WIN_WIDTH/(float)WIN_HEIGHT, //aspect ratio
											0.1f, //near plane distance from camera
											100.0f //far plane distance from near plane
											);

		
			//model transformation
			float angle = 20.0f;
			glm::vec3 translate_object_vec = glm::vec3(0.0f, -3.0f, 0.0f);
			gModelMatrix = glm::translate(gModelMatrix, 
										translate_object_vec //translate by
										);
			gModelMatrix = glm::scale(gModelMatrix, 
										glm::vec3(4.0f, 4.0f, 4.0f) //scale by
										);
			gModelMatrix = glm::rotate(gModelMatrix, //model matrix
				glm::radians(angle), //get time values
				glm::vec3(1.0f, 0.3f, 0.5f) //rotate around all axis
			);

			//pass values to uniform
			ourPlanetShader.setMat4("u_model_matrix", gModelMatrix);
			ourPlanetShader.setMat4("u_view_matrix", gViewMatrix);
			ourPlanetShader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);

			//draw
			ourPlanet.Draw(ourPlanetShader);


			ourShader.use();
			ourShader.setMat4("u_view_matrix", gViewMatrix);
			ourShader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);
			ourShader.setInt("texture_diffuse1", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ourAsteroid.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.

		//draw meteorites
			for (unsigned int i = 0; i < ourAsteroid.meshes.size(); i++)
			{
				glBindVertexArray(ourAsteroid.meshes[i].VAO);
				glDrawElementsInstanced(
					GL_TRIANGLES,
					ourAsteroid.meshes[i].indices.size(),
					GL_UNSIGNED_INT,
					0,
					amount
				);
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