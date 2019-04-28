/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 3 Nov 2018

	Program Name - Specular Maps with Error Debugging
	This is just a different name for the underlying principle of textures. In lighting, we call it as specular maps

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
GLenum glCheckError_(const char* file, int line);
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message, const void *userParam); //OpenGL callback API for error debugging

//macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define VIEWPORT_WIDTH 800
#define VIEWPORT_HEIGHT 600
#define glCheckError() glCheckError_(__FILE__, __LINE__)

//global variable declaration
//to transform all the vertices to the globle world space
glm::mat4 gModelMatrix; //it contains translation, rotation and scalling

glm::mat4 gViewMatrix; //translate the scene towards -ve z axis, this gives impression that we are moving backwards

glm::mat4 gPerspectiveProjectionMatrix; //for perspective projection

//set light position
glm::vec3 gLightPosition = glm::vec3(0.5f, -0.5f, 1.0f);

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

	/*glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); //requesting OpenGL debug context
	//check if we successfully initialized debug context
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); //directly calls callback function the moment an error occured
		glDebugMessageCallback(
			glDebugOutput, //callback function
			nullptr //user params
		);
		glDebugMessageControl(
			GL_DONT_CARE, //source: for filtering the type of error we would like to receive message from
			GL_DONT_CARE, //type
			GL_DONT_CARE, //severity
			0, //count
			nullptr, //ids
			GL_TRUE
		);
	}*/

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
	Shader lighting_shader("vertex_shader.vs", "fragment_shader.fs");
	Shader lamp_shader("vertex_shader.vs", "fragment_shader_lamp.fs");
	/*****************Build And Compile Our Shader Program Ends********************/

	//Input Vertex Data
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

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


	//texture data
	glVertexAttribPointer(
		2, //which vertex attribute we want to configure
		2, //size of the vertex attribute - 3 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		8 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *)(6 * sizeof(float)) //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(2); //enable vertex attribute; 2 is vertex attribute location

	//this is allowed; the call to glVertexAttribPointer registerd VBO as the vertex attribute's bound vertex buffer object;
	//so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vertex buffer

	//you can unbind the vao afterwards so that other VAO calls won't accidentaly modify this VAO
	glBindVertexArray(0);
	/*********Create Vertex Array Object for Cube ends********/


	/**********LOAD AND CREATE TEXTURE DATA STARTS*************/

	//diffuse map
	unsigned int diffuseMap;
	//generate texture
	glGenTextures(
				1, //how many texture objects you want to generate
				&diffuseMap //it will contain id of the texture generated over GPU
			);

	//glCheckError();

	//bind the above texture to the Texture type
	//all the upcoming GL_TEXTURE_2D operations will now affect this texture object
	glBindTexture(GL_TEXTURE_2D, //binding to this texture type
		diffuseMap //texture to bind
		);

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

	//LOAD IMAGE, CREATE TEXTURES AND GENERATE MIPMAPS
	int width, height, nrChannels;

	//load image data
	unsigned char* data = stbi_load(
							"container2.png", //file path
							&width, //this will contain width of the image
							&height, //this will contain height of the image
							&nrChannels, //this will contain number of colored channels
							0 //req_comp
						);

	//check if image data successfully loaded
	if (data) //if yes
	{
		//generate texture from this texture data
		glTexImage2D(
			GL_TEXTURE_2D, //target; this operation will generate texture on currently bound texture object
			0, //base mipmap level
			GL_RGB, //internal format; in what kind of format you want to store the texture
			width, //width of resulting texture
			height, //height of resulting texture
			0, //this should be always 0 for legacy purpose
			GL_RGBA, //format of source image; png has Alpha value also
			GL_UNSIGNED_BYTE, //data is in GL_UNSIGNED_BYTE format
			data //actual data
		);
		//glCheckError();
		//now the currently bound texture object has the texture image attached to it
		//but it only has the base-level of the texture image loaded
		//below will automatically generate all the required mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else //if no
	{
		std::cout << "ERROR::TEXTURE::LOADING_IMAGE_FAILED" << std::endl;
	}

	//free image data now
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture



	//specular map
	unsigned int specularMap;
	//generate texture
	glGenTextures(
		1, //how many texture objects you want to generate
		&specularMap //it will contain id of the texture generated over GPU
	);

	//bind the above texture to the Texture type
	//all the upcoming GL_TEXTURE_2D operations will now affect this texture object
	glBindTexture(GL_TEXTURE_2D, //binding to this texture type
		specularMap //texture to bind
	);

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

	//LOAD IMAGE, CREATE TEXTURES AND GENERATE MIPMAPS
	width = height = nrChannels = 0;

	//load image data
	data = nullptr;
	data = stbi_load(
		"container2_specular.png", //file path
		&width, //this will contain width of the image
		&height, //this will contain height of the image
		&nrChannels, //this will contain number of colored channels
		0 //req_comp
	);

	//check if image data successfully loaded
	if (data) //if yes
	{
		//generate texture from this texture data
		glTexImage2D(
			GL_TEXTURE_2D, //target; this operation will generate texture on currently bound texture object
			0, //base mipmap level
			GL_RGB, //internal format; in what kind of format you want to store the texture
			width, //width of resulting texture
			height, //height of resulting texture
			0, //this should be always 0 for legacy purpose
			GL_RGBA, //format of source image; png has Alpha value also
			GL_UNSIGNED_BYTE, //data is in GL_UNSIGNED_BYTE format
			data //actual data
		);

		//now the currently bound texture object has the texture image attached to it
		//but it only has the base-level of the texture image loaded
		//below will automatically generate all the required mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else //if no
	{
		std::cout << "ERROR::TEXTURE::SPECULAP MAP::LOADING_IMAGE_FAILED" << std::endl;
	}

	//free image data now
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture
	/**********LOAD AND CREATE TEXTURE DATA ENDS*************/


	/*****CREATE VERTEX ARRAY OBJECT FOR LAMP START*******/
	unsigned int vao_lamp;
	glGenVertexArrays(1, //how many vertex array objects 
		&vao_lamp //this will contain id of the vao created on GPU
		);
		
	//bind vertex array object
	glBindVertexArray(vao_lamp);


	//buffer objects can be of many types, but I want this buffer object to bound to Vertex Buffer
	//VBO's data already contains correct data
	glBindBuffer(GL_ARRAY_BUFFER, //type of buffer you want to bind 
				VBO //bind this vbo
				);

	//tell how this data is organized
	glVertexAttribPointer(0, //0th layout location attribute in the shader
		3, //3 float values for thsi attribute
		GL_FLOAT, //type of the values
		GL_FALSE, //normalization not needed
		8 * sizeof(float), //space between consecutive vertex attributes - stride
		(void *) 0 //from where position data begins in the buffer
		);

	//enable the 0th attribute
	glEnableVertexAttribArray(0);

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, //type of buffer you want to unbind 
		0 //0 means unbinding the buffer
	);

	//unbind vertex array object
	glBindVertexArray(0); //0 means unbinding the vao

	/*****CREATE VERTEX ARRAY OBJECT FOR LAMP ENDS*******/
	
	/*******Initialize some params**********/
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

		//clear the screens color buffer
		//here we pass in buffer bits to specify which buffer we would like to clear
		glClear(GL_COLOR_BUFFER_BIT //clear color buffer
				| GL_DEPTH_BUFFER_BIT //clear depth buffer
				);


		//draw our first triangle
		//activate shader program object
		lighting_shader.use();
		//object color
		glm::vec3 object_color = glm::vec3(1.0f, 0.5f, 0.31f);
		//light color
		glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
		//set uniform values
		//set light properties
		lighting_shader.setVec3("u_object_color", object_color);
		lighting_shader.setVec3("u_light_color", light_color);
		//set material properties
		lighting_shader.setInt("material.diffuse", 0); //set 0th texture unit
		lighting_shader.setInt("material.specular", 1); //set 01st texture unit
		lighting_shader.setFloat("material.shininess", 32.0f);

		//change light color
		//light_color.x = sin(glfwGetTime() * 2.0f);
		//light_color.y = sin(glfwGetTime() * 0.7f);
		//light_color.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = light_color * glm::vec3(0.5f); //decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); //low influence

		lighting_shader.setVec3("light.ambient", ambientColor);
		lighting_shader.setVec3("light.diffuse", diffuseColor);
		lighting_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
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
		lighting_shader.setMat4("u_view_matrix", gViewMatrix);
		lighting_shader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);
		lighting_shader.setVec3("u_light_position", gLightPosition);
		lighting_shader.setVec3("u_viewer_position", camera.Position);
		//bind which VAO we are going to use with the program object
		glBindVertexArray(VAO);

		//activate texture unit - diffuse
		glActiveTexture(GL_TEXTURE0); //0th texture unit

		//bind the texture now
		glBindTexture(
			GL_TEXTURE_2D, //texture target
			diffuseMap //texture value
		);


		//activate texture unit - specular
		glActiveTexture(GL_TEXTURE1); //1th texture unit

		//bind the texture now
		glBindTexture(
			GL_TEXTURE_2D, //texture target
			specularMap //texture value
		);
		//draw cubes with different model matrices
		
			gModelMatrix = glm::mat4(); //set to identity
			
			float angle = 20.0f;
			glm::vec3 translate_object_vec = glm::vec3(0.0f, 0.0f, -0.5f);
			gModelMatrix = glm::translate(gModelMatrix, translate_object_vec);
			gModelMatrix = glm::rotate(gModelMatrix, //model matrix
				glm::radians(angle), //get time values
				glm::vec3(1.0f, 0.3f, 0.5f) //rotate around all axis
			);
			lighting_shader.setMat4("u_model_matrix", gModelMatrix);

			//draw cube
			glDrawArrays(
				GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
				0, //starting index of enabled vertex arrays
				36 //how many vertices you want to draw
			);
		
		

		glBindVertexArray(0); //unbind the VAO

		/*********Draw LAMP START**************/
		lamp_shader.use();
		//set transformation matrices
		gModelMatrix = glm::mat4(); //set to identity
		gModelMatrix = glm::translate(gModelMatrix, gLightPosition);
		gModelMatrix = glm::scale(gModelMatrix, glm::vec3(0.2f));

		//pass uniforms
		lamp_shader.setMat4("u_model_matrix", gModelMatrix);
		lamp_shader.setMat4("u_view_matrix", gViewMatrix);
		lamp_shader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);
		//bind vao
		glBindVertexArray(vao_lamp);
		//draw cube
		glDrawArrays(
			GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
			0, //starting index of enabled vertex arrays
			36 //how many vertices you want to draw
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

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}

		std::cout << error << " | " << file << " ( " << line << " ) " << std::endl;
	}
	return errorCode;
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message, const void *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "Debug message (" << id << "): " << message << std::endl;
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	}
	std::cout << std::endl;
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	}
	std::cout << std::endl;
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}