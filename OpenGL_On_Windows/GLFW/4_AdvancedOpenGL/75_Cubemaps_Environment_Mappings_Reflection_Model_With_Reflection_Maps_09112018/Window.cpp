/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 9 Nov 2018

	Program Name - Advanced OpenGL - Cubemaps
	Environment Mappings - Reflection
	Loaded Models with reflection map

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

//to load the model
#include "model.h"

#include<vector>

using namespace std;

//global function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path); //to load textures
unsigned int loadCubemap(vector<std::string> faces);

//macros
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define VIEWPORT_WIDTH 1280
#define VIEWPORT_HEIGHT 720

//global variable declaration
//to transform all the vertices to the globle world space
glm::mat4 gModelMatrix; //it contains translation, rotation and scalling
glm::mat4 gViewMatrix; //translate the scene towards -ve z axis, this gives impression that we are moving backwards
glm::mat4 gPerspectiveProjectionMatrix; //for perspective projection

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

	Shader skyboxShader("cube_map.vs", "cube_map.fs");

	Model ourModel("resources/objects/nanosuit/nanosuit.obj");
	/*****************Build And Compile Our Shader Program Ends********************/

	//Input Vertex Data
	float cubeVertices[] = {
		// positions          // normals
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

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	//for skybox
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
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
					6 * sizeof(float), //space between consecutive vertex attribtes - stride
					(void *) 0 //from where the position data begins in the buffer
				);

	glEnableVertexAttribArray(0); //enable vertex attribute; 0 is vertex attribute location

	//normals data
	glVertexAttribPointer(
		1, //which vertex attribute we want to configure
		3, //size of the vertex attribute - 3 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		6 * sizeof(float), //space between consecutive vertex attribtes - stride
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


	/***********Setup SKYBOX VAO START***********/
	//create vertex array object
	//a VAO that stores our vertex attribute configuration
	//and which VBO to use
	unsigned int VAO_Skybox, VBO_Skybox;
	glGenVertexArrays(1,  //how many vertex array objects you want to create
		&VAO_Skybox //id of vertex array object is store in this
	);
	//bind vertex array object first, then bind and set vertex buffers
	//and then configure vertex attributes
	glBindVertexArray(VAO_Skybox);

	//create memory on GPU - vertex buffer object
	glGenBuffers(
		1, //how many buffers you want to create
		&VBO_Skybox //this will contain the buffer id of the created buffer on GPU
	);

	//here onwards, any buffer call(on GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer, VBO
	glBindBuffer(
		GL_ARRAY_BUFFER, //type of created buffer on GPU
		VBO_Skybox //created buffer
	);

	//copy previously defined data to the buffer's memory
	glBufferData(
		GL_ARRAY_BUFFER, //this type of buffer is your target, which is VBO
		sizeof(skyboxVertices), //size of the data which we will pass
		skyboxVertices, //actual data
		GL_STATIC_DRAW //specify how you want graphics card will manage the data in the buffer
	);


	//tell OpenGL that how to link vertex data to vertex attributes in shader
	//position data
	glVertexAttribPointer(
		0, //which vertex attribute we want to configure
		3, //size of the vertex attribute - 3 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		3 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *)0 //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(0); //enable vertex attribute; 0 is vertex attribute location

	//this is allowed; the call to glVertexAttribPointer registerd VBO as the vertex attribute's bound vertex buffer object;
	//so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vertex buffer

	//you can unbind the vao afterwards so that other VAO calls won't accidentaly modify this VAO
	glBindVertexArray(0);
	/********************Setup SKYBOX VAO END*******************/
	/***********Setup VAO END***********/

	/**********LOAD AND CREATE TEXTURE DATA STARTS*************/
	//cube texture
	unsigned int cubeTexture = loadTexture("resources/textures/container.jpg");

	//floor texture
	unsigned int planeTexture = loadTexture("resources/textures/metal.png");

	//cubemap textures
	vector<std::string> faces
	{
		"resources/textures/cubemaps/skybox/right.jpg",
		"resources/textures/cubemaps/skybox/left.jpg",
		"resources/textures/cubemaps/skybox/top.jpg",
		"resources/textures/cubemaps/skybox/bottom.jpg",
		"resources/textures/cubemaps/skybox/front.jpg",
		"resources/textures/cubemaps/skybox/back.jpg"
		/*"resources/textures/cubemaps/ame_oasis/oasisnight_rt.tga",
		"resources/textures/cubemaps/ame_oasis/oasisnight_lf.tga",
		"resources/textures/cubemaps/ame_oasis/oasisnight_up.tga",
		"resources/textures/cubemaps/ame_oasis/oasisnight_dn.tga",
		"resources/textures/cubemaps/ame_oasis/oasisnight_ft.tga",
		"resources/textures/cubemaps/ame_oasis/oasisnight_bk.tga"*/
	};

	unsigned int cubemapTexture = loadCubemap(faces);
	/**********LOAD AND CREATE TEXTURE DATA ENDS*************/

	/*******Configure OpenGL states**********/
	//enable depth testing - every fragments depth value (gl_FragCoord's z value) is compared with the value in the depth buffer
	glEnable(GL_DEPTH_TEST);

	//set condition to make depth test pass
	glDepthFunc(GL_LEQUAL);

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

		/**********CREATE TRANSFORMATION MATRICES***************/
		//load transformation matrices to identity matrix
		gModelMatrix = glm::mat4();
		gViewMatrix = glm::mat4();
		gPerspectiveProjectionMatrix = glm::mat4();

		//calculate new delta time value within each frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		

		//draw Cube
		//activate shader program object
		ourShader.use();
		//set sampler - texture unit
		ourShader.setInt("u_skybox_sampler",5);
		ourShader.setVec3("u_camera_position", camera.Position);

		//load transformation matrices to identity matrix
		gModelMatrix = glm::mat4();
		gViewMatrix = glm::mat4();
		gPerspectiveProjectionMatrix = glm::mat4();

		//view transformation
		gViewMatrix = camera.GetViewMatrix();

		//perspective transformation
		gPerspectiveProjectionMatrix = glm::perspective(
			glm::radians(camera.Zoom), //fovy
			(float)giWidth / (float)giHeight, //aspect ratio
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
			glm::vec3(0.2f, 0.2f, 0.2f) //scale by
		);
		/*gModelMatrix = glm::rotate(gModelMatrix, //model matrix
		glm::radians(angle), //get time values
		glm::vec3(1.0f, 0.3f, 0.5f) //rotate around all axis
		);*/

		//view transformation
		//gViewMatrix = camera.GetViewMatrix();

		//pass uniforms
		ourShader.setMat4("u_model_matrix", gModelMatrix);
		ourShader.setMat4("u_view_matrix", gViewMatrix);
		ourShader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);
		
		//activate texture unit
		glActiveTexture(GL_TEXTURE5); //4th texture unit

		//bind the texture now
		glBindTexture(
			GL_TEXTURE_CUBE_MAP, //texture target
			cubemapTexture //texture value
		);

		ourModel.Draw(ourShader);


		glBindVertexArray(0); //unbind the VAO

		glUseProgram(0); //unbind the currently bounded program object


		/***********Draw SKYBOX starts***********/
		glDepthMask(GL_FALSE); //disable writing to depth buffer

		skyboxShader.use(); //activate skybox shader program object

		skyboxShader.setInt("u_skybox_cubemap_sampler",0);
		gModelMatrix = glm::mat4(); //set to identity

		//view transformation
		gViewMatrix = glm::mat4(glm::mat3(camera.GetViewMatrix()));

		skyboxShader.setMat4("u_model_matrix", gModelMatrix);
		skyboxShader.setMat4("u_view_matrix", gViewMatrix);
		skyboxShader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);

		//bind skybox VAO to use
		glBindVertexArray(VAO_Skybox);

		//bind cubemap texture
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		//draw cubemap
		glDrawArrays(
			GL_TRIANGLES, //use primitive
			0, //starts from
			36 //count of vertices to draw
		);

		glBindVertexArray(0); //unbind skybox vao

		glUseProgram(0); //deactivate skybox shader program object

		glDepthMask(GL_TRUE); //enable writing to depth buffer
		/***********Draw SKYBOX ends***********/

		//swap color buffers - a large buffer that contains the color values for each pixel's in GLFW window
		//that has been used to draw in during this iteration and show it as the output to the screen
		glfwSwapBuffers(window);

		//check if any events are triggered
		glfwPollEvents();
	}

	//de-allocate all the resources once they are not in use
	glDeleteVertexArrays(1, &VAO_Cube); //cube vao
	glDeleteVertexArrays(1, &VAO_Plane); //floor cube
	glDeleteVertexArrays(1, &VAO_Skybox); //skybox
	glDeleteBuffers(1, &VBO_Cube); //cube vbo
	glDeleteBuffers(1, &VBO_Plane); //plane vbo
	glDeleteBuffers(1, &VBO_Skybox); //skybox sbo

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

//loadCubemap: create the cubemap from the faces provided and return it's id
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;

	//create texture in memory
	glGenTextures(
		1, //how many
		&textureID //stores the id of the generated texture object
	);

	//bind texture to the texture type
	glBindTexture(
		GL_TEXTURE_CUBE_MAP, //target
		textureID
	);

	//load image data and generate texture from it
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		//load image data
		unsigned char* data = stbi_load(
									faces[i].c_str(), //path of the file
									&width, //store width of image
									&height, //stores height of the image
									&nrChannels, //stores image channels e.g. RGB
									0 //req_comp
								);

		//if data loaded
		if (data)
		{
			//derive the color components of the image
			GLenum format;
			if (nrChannels == 1)
			{
				format = GL_RED;
			}else if (nrChannels == 3)
			{
				format = GL_RGB;
			}
			else if (nrChannels == 4)
			{
				format = GL_RGBA;
			}

			//generate texture for six sides of the cubemap
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, //target
				0, //mipmap level
				format, //internal format
				width, //width of texture
				height, //height of the texture
				0, //for legacy purpose
				format, //image format
				GL_UNSIGNED_BYTE, //data type of the image data
				data //actual data
			);
			

		}
		else //if not
		{
			std::cout << "OpenGL::TEXTURE::CUBEMAPS:: cubemap texture failed to load at path: " << faces[i] << std::endl;
		}

		//free image data
		stbi_image_free(data);
	}


	//set filtering parameters of the texture
	glTexParameteri(
		GL_TEXTURE_CUBE_MAP, //target
		GL_TEXTURE_MIN_FILTER, //property - for scroll down
		GL_LINEAR //value
	);

	glTexParameteri(
		GL_TEXTURE_CUBE_MAP, //target
		GL_TEXTURE_MAG_FILTER, //property - for scroll up
		GL_LINEAR //value
	);

	//set wrapping parameters
	glTexParameteri(
		GL_TEXTURE_CUBE_MAP, //target
		GL_TEXTURE_WRAP_S, //property - s texcoord
		GL_CLAMP_TO_EDGE //value
	);

	glTexParameteri(
		GL_TEXTURE_CUBE_MAP, //target
		GL_TEXTURE_WRAP_T, //property - t texcoord
		GL_CLAMP_TO_EDGE //value
	);

	glTexParameteri(
		GL_TEXTURE_CUBE_MAP, //target
		GL_TEXTURE_WRAP_R, //property - r texcoord
		GL_CLAMP_TO_EDGE //value
	);

	return textureID;
}