/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 29 Dec 2018
	
	Program Name - Physically Based Rendering

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

//set light position
glm::vec3 gLightPosition = glm::vec3(1.2f, 1.0f, 0.0f);

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

//FBO, VAO, VBO, EBO, TBO
unsigned int sphereVAO, sphereVBO, sphereEBO; //for spheres
unsigned int indexCount;

glm::vec3 gLightPositions[] = {
	glm::vec3(0.0f,  0.0f, 10.0f)
};
glm::vec3 gLightColors[] = {
	glm::vec3(150.0f, 150.0f, 150.0f)
};
int nrRows = 7;
int nrColumns = 7;
float spacing = 2.5;

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
	Shader lighting_shader("vertex_shader.vs", "fragment_shader.fs");
	/*****************Build And Compile Our Shader Program Ends********************/

	/*
	Data Initialization Block Starts
	---------------------------------	
	*/
	//Input Vertex Data
	float vertices[] = {
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

	//Generate sphere data -starts
	std::vector<glm::vec3> positionSphere;
	std::vector<glm::vec2> uvSphere;
	std::vector<glm::vec3> normalSphere;
	std::vector<unsigned int> indicesSphere;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;

	//attribute generation
	for (unsigned int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; x++)
		{
			//uv's
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			//positions
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positionSphere.push_back(glm::vec3(xPos, yPos, zPos));
			uvSphere.push_back(glm::vec2(xSegment, ySegment));
			normalSphere.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	//indices generation
	bool oddRow = false;
	for (int y = 0; y < Y_SEGMENTS; y++)
	{
		if (!oddRow) //even row
		{
			for (int x = 0; x <= X_SEGMENTS; x++)
			{
				indicesSphere.push_back(y * (X_SEGMENTS + 1) + x);
				indicesSphere.push_back((y+1)* (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; x--)
			{
				indicesSphere.push_back((y + 1)* (X_SEGMENTS + 1) + x);
				indicesSphere.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	indexCount = indicesSphere.size();

	//data generation
	std::vector<float> data;
	for (int i = 0; i < positionSphere.size(); i++)
	{
		//positions
		data.push_back(positionSphere[i].x);
		data.push_back(positionSphere[i].y);
		data.push_back(positionSphere[i].z);
		//tex-coords
		if (uvSphere.size() > 0)
		{
			data.push_back(uvSphere[i].x);
			data.push_back(uvSphere[i].y);
		}
		//normals
		if (normalSphere.size() > 0)
		{
			data.push_back(normalSphere[i].x);
			data.push_back(normalSphere[i].y);
			data.push_back(normalSphere[i].z);
		}
	}
	//Generate sphere data -ends

	//load Material Textures
	unsigned int albedo = loadTexture(
		"resources/textures/pbr/rustediron2_basecolor.png", //path
		false //gamma correction
	);
	unsigned int metallic = loadTexture(
		"resources/textures/pbr/rustediron2_metallic.png", //path
		false //gamma correction
	);
	unsigned int normal = loadTexture(
		"resources/textures/pbr/rustediron2_normal.png", //path
		false //gamma correction
	);
	unsigned int roughness = loadTexture(
		"resources/textures/pbr/rustediron2_roughness.png", //path
		false //gamma correction
	);
	unsigned int ao = loadTexture(
		"resources/textures/pbr/rustediron2_ao.png", //path
		false //gamma correction
	);

	//set uniforms
	lighting_shader.use();
	lighting_shader.setInt("u_albedo_map", 0);
	lighting_shader.setInt("u_normal_map", 1);
	lighting_shader.setInt("u_mettalic_map", 2);
	lighting_shader.setInt("u_roughness_map", 3);
	lighting_shader.setInt("u_ao_map", 4);
	/*
	Data Initialization Block Ends
	---------------------------------
	*/

	/*
	Configuration of FBO,VAO,VBO,TBO,EBO begins
	---------------------------------------------
	*/
	//a VAO that stores our vertex attribute configuration
	//and which VBO to use
	glGenVertexArrays(1,  //how many vertex array objects you want to create
					&sphereVAO //id of vertex array object is store in this
				);
	//bind vertex array object first, then bind and set vertex buffers
	//and then configure vertex attributes
	glBindVertexArray(sphereVAO);

	//create memory on GPU - vertex buffer object
	glGenBuffers(
			1, //how many buffers you want to create
			&sphereVBO //this will contain the buffer id of the created buffer on GPU
			);

	//here onwards, any buffer call(on GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer, VBO
	glBindBuffer(
			GL_ARRAY_BUFFER, //type of created buffer on GPU
		sphereVBO //created buffer
			);

	//copy previously defined data to the buffer's memory
	glBufferData(
				GL_ARRAY_BUFFER, //this type of buffer is your target, which is VBO
				data.size() * sizeof(GL_FLOAT), //size of the data which we will pass
				&data[0], //actual data
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

	//texture data
	glVertexAttribPointer(
		1, //which vertex attribute we want to configure
		2, //size of the vertex attribute - 2 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		8 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *) (3 * sizeof(GL_FLOAT)) //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(1); //enable vertex attribute; 1 is vertex attribute location

	//normal data
	glVertexAttribPointer(
		2, //which vertex attribute we want to configure
		3, //size of the vertex attribute - 3 float values for each vertex
		GL_FLOAT, //type of the data
		GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
		8 * sizeof(float), //space between consecutive vertex attribtes - stride
		(void *) (5 * sizeof(GL_FLOAT)) //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(2); //enable vertex attribute; 2 is vertex attribute location

	//this is allowed; the call to glVertexAttribPointer registerd VBO as the vertex attribute's bound vertex buffer object;
	//so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vertex buffer


	//create and bind EBO
	glGenBuffers(1, &sphereEBO); //create
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO); //bind
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSphere.size() * sizeof(unsigned int), &indicesSphere[0], GL_STATIC_DRAW);

	//you can unbind the vao afterwards so that other VAO calls won't accidentaly modify this VAO
	glBindVertexArray(0);

	/*
	Configuration of FBO,VAO,VBO,TBO,EBO ends
	---------------------------------------------
	*/

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

		//clear the screens color buffer
		//here we pass in buffer bits to specify which buffer we would like to clear
		glClear(GL_COLOR_BUFFER_BIT //clear color buffer
				| GL_DEPTH_BUFFER_BIT //clear depth buffer
				);


		//draw our first triangle
		//activate shader program object
		lighting_shader.use();
		
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
		lighting_shader.setVec3("u_viewer_position", camera.Position);
		//bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallic);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughness);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ao);

		//draw cubes with different model matrices
		
			gModelMatrix = glm::mat4(); //set to identity
			for (int row = 0; row < nrRows; ++row)
			{
				
				for (int col = 0; col < nrColumns; col++)
				{
					
					gModelMatrix = glm::mat4(); //set to identity
					gModelMatrix = glm::translate(gModelMatrix,
					glm::vec3((col - (nrColumns/2)) * spacing, (row - (nrRows/2)) * spacing, 0.0f)
					);
					lighting_shader.setMat4("u_model_matrix", gModelMatrix);

					//render sphere
					//bind which VAO we are going to use with the program object
					glBindVertexArray(sphereVAO);
					//draw cube
					glDrawElements(
						GL_TRIANGLE_STRIP, //what we want to draw using the vertices given in the shader program object
						indexCount,
						GL_UNSIGNED_INT,
						0
					);
					glBindVertexArray(0); //unbind the VAO
				}
			}
			
		
			// render light source (simply re-render sphere at light positions)
			// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
			// keeps the codeprint small.
			for (unsigned int i = 0; i < sizeof(gLightPositions) / sizeof(gLightPositions[0]); ++i)
			{
				glm::vec3 newPos = gLightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
				newPos = gLightPositions[i];
				lighting_shader.setVec3("u_light_positions[" + std::to_string(i) + "]", newPos);
				lighting_shader.setVec3("u_light_colors[" + std::to_string(i) + "]", gLightColors[i]);

				gModelMatrix = glm::mat4();
				gModelMatrix = glm::translate(gModelMatrix, newPos);
				gModelMatrix = glm::scale(gModelMatrix, glm::vec3(0.5f));
				lighting_shader.setMat4("u_model_matrix", gModelMatrix);
				//render sphere
				//bind which VAO we are going to use with the program object
				glBindVertexArray(sphereVAO);
				//draw cube
				glDrawElements(
					GL_TRIANGLE_STRIP, //what we want to draw using the vertices given in the shader program object
					indexCount,
					GL_UNSIGNED_INT,
					0
				);
				glBindVertexArray(0); //unbind the VAO
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

//loadTexture: Loads the texture from the given path
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
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
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
			dataFormat, //image format
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