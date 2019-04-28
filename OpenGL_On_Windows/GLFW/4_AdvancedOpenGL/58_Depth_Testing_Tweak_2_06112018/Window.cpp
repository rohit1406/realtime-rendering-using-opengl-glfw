/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 6 Nov 2018

	Program Name - Advanced OpenGL - Depth Testing
	Visualising Depth Buffer -
	Depth values range between [0,1], non-linearly
	Convert Non-linearized values to linear, of each fragment.
	The colors are mostly black because the depth values range linearly from the near 
	plane which is 0.1 to the far plane which is 100 that is still quite far away 
	from us. The result is that we're relatively close to the near plane and thus get 
	lower (darker) depth values.


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
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path); //to load textures

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
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f) //position
				);

//mouse movements
bool firstMouse = true;
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;

//fullscreen
bool gbFullScreen = false;

//to adjust viewport size while resizing
int giWidth, giHeight;


//entry-point function
int main()
{
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
	Shader ourShader("vertex_shader.vs", "fragment_shader.fs");
	/*****************Build And Compile Our Shader Program Ends********************/

	//Input Vertex Data
	float cubeVertices[] = {
		// positions          // texture Coords
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

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	/***********Setup VAO START***********/
	/***********Setup Cube VAO START***********/
	//create vertex array object
	//a VAO that stores our vertex attribute configuration
	//and which VBO to use
	unsigned int VAO_Cube, VBO_Cube;
	glGenVertexArrays(1,  //how many vertex array objects you want to create
					&VAO_Cube //id of vertex array object is store in this
				);
	//bind vertex array object first, then bind and set vertex buffers
	//and then configure vertex attributes
	glBindVertexArray(VAO_Cube);

	//create memory on GPU - vertex buffer object
	glGenBuffers(
			1, //how many buffers you want to create
			&VBO_Cube //this will contain the buffer id of the created buffer on GPU
			);

	//here onwards, any buffer call(on GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer, VBO
	glBindBuffer(
			GL_ARRAY_BUFFER, //type of created buffer on GPU
			VBO_Cube //created buffer
			);

	//copy previously defined data to the buffer's memory
	glBufferData(
				GL_ARRAY_BUFFER, //this type of buffer is your target, which is VBO
				sizeof(cubeVertices), //size of the data which we will pass
				cubeVertices, //actual data
				GL_STATIC_DRAW //specify how you want graphics card will manage the data in the buffer
			);

	
	//tell OpenGL that how to link vertex data to vertex attributes in shader
	//position data
	glVertexAttribPointer(
					0, //which vertex attribute we want to configure
					3, //size of the vertex attribute - 3 float values for each vertex
					GL_FLOAT, //type of the data
					GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
					5 * sizeof(float), //space between consecutive vertex attribtes - stride
					(void *) 0 //from where the position data begins in the buffer
				);

	glEnableVertexAttribArray(0); //enable vertex attribute; 0 is vertex attribute location

	//texture data
	glVertexAttribPointer(
		1, //which vertex attribute we want to configure
		2, //size of the vertex attribute - 3 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		5 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *) (3 * sizeof(float)) //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(1); //enable vertex attribute; 1 is vertex attribute location

	//this is allowed; the call to glVertexAttribPointer registerd VBO as the vertex attribute's bound vertex buffer object;
	//so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vertex buffer

	//you can unbind the vao afterwards so that other VAO calls won't accidentaly modify this VAO
	glBindVertexArray(0);
	/********************Setup Cube VAO END*******************/

	/********************Setup PLANE VAO START*******************/
	//create vertex array object
	//a VAO that stores our vertex attribute configuration
	//and which VBO to use
	unsigned int VAO_Plane, VBO_Plane;
	glGenVertexArrays(1,  //how many vertex array objects you want to create
		&VAO_Plane //id of vertex array object is store in this
	);
	//bind vertex array object first, then bind and set vertex buffers
	//and then configure vertex attributes
	glBindVertexArray(VAO_Plane);

	//create memory on GPU - vertex buffer object
	glGenBuffers(
		1, //how many buffers you want to create
		&VBO_Plane //this will contain the buffer id of the created buffer on GPU
	);

	//here onwards, any buffer call(on GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer, VBO
	glBindBuffer(
		GL_ARRAY_BUFFER, //type of created buffer on GPU
		VBO_Plane //created buffer
	);

	//copy previously defined data to the buffer's memory
	glBufferData(
		GL_ARRAY_BUFFER, //this type of buffer is your target, which is VBO
		sizeof(planeVertices), //size of the data which we will pass
		planeVertices , //actual data
		GL_STATIC_DRAW //specify how you want graphics card will manage the data in the buffer
	);


	//tell OpenGL that how to link vertex data to vertex attributes in shader
	//position data
	glVertexAttribPointer(
		0, //which vertex attribute we want to configure
		3, //size of the vertex attribute - 3 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		5 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *)0 //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(0); //enable vertex attribute; 0 is vertex attribute location

	//texture data
	glVertexAttribPointer(
		1, //which vertex attribute we want to configure
		2, //size of the vertex attribute - 3 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		5 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *)(3 * sizeof(float)) //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(1); //enable vertex attribute; 1 is vertex attribute location

	//this is allowed; the call to glVertexAttribPointer registerd VBO as the vertex attribute's bound vertex buffer object;
	//so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vertex buffer

	//you can unbind the vao afterwards so that other VAO calls won't accidentaly modify this VAO
	glBindVertexArray(0);
	/***********Setup PLANE VAO END*******************/
	/***********Setup VAO END***********/

	/**********LOAD AND CREATE TEXTURE DATA STARTS*************/
	//diffuse map
	unsigned int cubeTexture = loadTexture("resources/textures/marble.jpg");

	//specular map
	unsigned int planeTexture = loadTexture("resources/textures/metal.png");

	/**********LOAD AND CREATE TEXTURE DATA ENDS*************/

	/*******Configure OpenGL states**********/
	//enable depth testing - every fragments depth value (gl_FragCoord's z value) is compared with the value in the depth buffer
	glEnable(GL_DEPTH_TEST);

	//set condition to make depth test pass
	glDepthFunc(GL_LESS); //default

	//tell OpenGL the size of the rendering window
	glViewport(0, 0,  //location of the lower left corner of the window
			VIEWPORT_WIDTH, VIEWPORT_HEIGHT //width and height of the rendering window
		);
	giWidth = WIN_WIDTH;
	giHeight = WIN_HEIGHT;

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
		ourShader.use();
		//set sampler - texture unit
		ourShader.setInt("u_texture0",0);

		/**********CREATE TRANSFORMATION MATRICES***************/
		//load transformation matrices to identity matrix
		gModelMatrix = glm::mat4();
		gViewMatrix = glm::mat4();
		gPerspectiveProjectionMatrix = glm::mat4();
		
		//calculate new delta time value within each frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//model transformation
		glm::vec3 translate_object_vec = glm::vec3(-1.0f, 0.0f, -1.0f);
		gModelMatrix = glm::translate(gModelMatrix, //model matrix
			translate_object_vec //translation vector
		);

		//view transformation
		gViewMatrix = camera.GetViewMatrix();

		//perspective transformation
		gPerspectiveProjectionMatrix = glm::perspective(
											glm::radians(camera.Zoom), //fovy
											(float)giWidth/(float)giHeight, //aspect ratio
											0.1f, //near plane distance from camera
											100.0f //far plane distance from near plane
											);

		//pass uniforms
		ourShader.setMat4("u_model_matrix", gModelMatrix);
		ourShader.setMat4("u_view_matrix", gViewMatrix);
		ourShader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);
		//bind which VAO we are going to use with the program object
		glBindVertexArray(VAO_Cube);

		//activate texture unit
		glActiveTexture(GL_TEXTURE0); //0th texture unit

		//bind the texture now
		glBindTexture(
			GL_TEXTURE_2D, //texture target
			cubeTexture //texture value
		);

			//draw cube
			glDrawArrays(
				GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
				0, //starting index of enabled vertex arrays
				36 //how many vertices you want to draw
			);

			//draw another cube
			//perform model transformation
			gModelMatrix = glm::mat4(); //set to identity
			translate_object_vec = glm::vec3(2.0f, 0.0f, 0.0f);
			gModelMatrix = glm::translate(gModelMatrix, //model matrix
				translate_object_vec //translation vector
			);

			//pass uniforms
			ourShader.setMat4("u_model_matrix", gModelMatrix);

			//draw cube
			glDrawArrays(
				GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
				0, //starting index of enabled vertex arrays
				36 //how many vertices you want to draw
			);


		glBindVertexArray(0); //unbind the VAO


		//draw floor
		//bind which VAO we are going to use with the program object - here drawing floor
		glBindVertexArray(VAO_Plane);

		//model transformation
		gModelMatrix = glm::mat4(); //set to identity

		//pass uniforms
		ourShader.setMat4("u_model_matrix", gModelMatrix);

		//activate texture unit
		glActiveTexture(GL_TEXTURE0); //0th texture unit

		//bind the texture now
		glBindTexture(
			GL_TEXTURE_2D, //texture target
			planeTexture //texture value
		);

		//draw cube
		glDrawArrays(
			GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
			0, //starting index of enabled vertex arrays
			6 //how many vertices you want to draw
		);

		glBindVertexArray(0); //unbind the VAO

		//swap color buffers - a large buffer that contains the color values for each pixel's in GLFW window
		//that has been used to draw in during this iteration and show it as the output to the screen
		glfwSwapBuffers(window);

		//check if any events are triggered
		glfwPollEvents();
	}

	//de-allocate all the resources once they are not in use
	glDeleteVertexArrays(1, &VAO_Cube); //cube vao
	glDeleteVertexArrays(1, &VAO_Plane); //floor cube
	glDeleteBuffers(1, &VBO_Cube); //cube vbo
	glDeleteBuffers(1, &VBO_Plane); //plane vbo

	//glfw: clean/delete all GLFW's resources that were allocated
	glfwTerminate();

	return 0;
}

/*  framebuffer_size_callback: callback function on the window that gets called each time the window is resized.
	Because, the moment user resizes the window, the viewport should be adjusted as well
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (height == 0) {
		height = 1;
	}
	//tell OpenGL the size of the rendering window
	glViewport(0, 0,  //location of the lower left corner of the window
		width, height //width and height of the rendering window
	);

	giWidth = width;
	giHeight = height;
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
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //hide cursor

			//set window size to fullscreen
			glfwSetWindowSize(window, mode->width, mode->height);
			glfwSetWindowPos(window, 0, 0);
		}
		else
		{
			gbFullScreen = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //show cursor
			glfwSetWindowSize(window, WIN_WIDTH, WIN_HEIGHT);
			//position window to center
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