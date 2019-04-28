/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 4 Nov 2018

	Program Name - Multiple Lights
	a light source with position somewhere in the environment that instead of shooting the light rays in all the
	direction, only shoots them in a specific direction
	e.g. street lamp or flash light
	In OpenGL, it is represented by a world-space position, a direction and a cutoff angle that specifies the
	radius of the spot light
	Flash light is a spot light located at the viewers position and usually aimed straight from the players perspective

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
unsigned int loadTexture(char const* path);

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
glm::vec3 gLightPosition = glm::vec3(1.5f, 1.5f, 2.0f);

//cube positions
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
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

//position vector of point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};
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

int giWidth, giHeight;


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
	unsigned int diffuseMap = loadTexture("container2.png");

	//specular map
	unsigned int specularMap = loadTexture("container2_specular.png");

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
		lighting_shader.setInt("material.emission", 2); //set 2nd texture unit
		lighting_shader.setFloat("material.shininess", 64.0f);

		//change light color
		//light_color.x = sin(glfwGetTime() * 2.0f);
		//light_color.y = sin(glfwGetTime() * 0.7f);
		//light_color.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = light_color * glm::vec3(0.5f); //decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); //low influence

		//set directional light uniforms
		lighting_shader.setVec3("dirLight.ambient", ambientColor);
		lighting_shader.setVec3("dirLight.diffuse", diffuseColor);
		lighting_shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
		lighting_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);

		
			//set point light uniforms
			lighting_shader.setVec3("pointLight[0].ambient", ambientColor);
			lighting_shader.setVec3("pointLight[0].diffuse", diffuseColor);
			lighting_shader.setVec3("pointLight[0].specular", 1.0f, 1.0f, 1.0f);
			lighting_shader.setVec3("pointLight[0].position", pointLightPositions[0]);
			lighting_shader.setFloat("pointLight[0].constant", 1.0f);
			lighting_shader.setFloat("pointLight[0].linear", 0.09f);
			lighting_shader.setFloat("pointLight[0].quadratic", 0.032f);

			lighting_shader.setVec3("pointLight[1].ambient", ambientColor);
			lighting_shader.setVec3("pointLight[1].diffuse", diffuseColor);
			lighting_shader.setVec3("pointLight[1].specular", 1.0f, 1.0f, 1.0f);
			lighting_shader.setVec3("pointLight[1].position", pointLightPositions[1]);
			lighting_shader.setFloat("pointLight[1].constant", 1.0f);
			lighting_shader.setFloat("pointLight[1].linear", 0.09f);
			lighting_shader.setFloat("pointLight[1].quadratic", 0.032f);

			lighting_shader.setVec3("pointLight[2].ambient", ambientColor);
			lighting_shader.setVec3("pointLight[2].diffuse", diffuseColor);
			lighting_shader.setVec3("pointLight[2].specular", 1.0f, 1.0f, 1.0f);
			lighting_shader.setVec3("pointLight[2].position", pointLightPositions[2]);
			lighting_shader.setFloat("pointLight[2].constant", 1.0f);
			lighting_shader.setFloat("pointLight[2].linear", 0.09f);
			lighting_shader.setFloat("pointLight[2].quadratic", 0.032f);

			lighting_shader.setVec3("pointLight[3].ambient", ambientColor);
			lighting_shader.setVec3("pointLight[3].diffuse", diffuseColor);
			lighting_shader.setVec3("pointLight[3].specular", 1.0f, 1.0f, 1.0f);
			lighting_shader.setVec3("pointLight[3].position", pointLightPositions[3]);
			lighting_shader.setFloat("pointLight[3].constant", 1.0f);
			lighting_shader.setFloat("pointLight[3].linear", 0.09f);
			lighting_shader.setFloat("pointLight[3].quadratic", 0.032f);
		
		//in fragment shader, we are calculating the dot product of light_direction and spot_direction and this dot product
		//returns cosine value and not the angle, so we can't direction compare angle with a cosine value. To retrieve angle
		//we must make inverse of cosine which is expensive to calculate per fragment
		//so we calculate cosine and pass it to fragment shader
		
		//lighting_shader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
		//lighting_shader.setFloat("light.outercutoff", glm::cos(glm::radians(17.5f)));

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
											(float)giWidth/(float)giHeight, //aspect ratio
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
		for (unsigned int i = 0; i < _ARRAYSIZE(cubePositions); i++)
		{
			//model transformation
			gModelMatrix = glm::mat4(); //set to identity

			float angle = i * 20.0f;
			//glm::vec3 translate_object_vec = glm::vec3(0.0f, 0.0f, -0.5f);
			gModelMatrix = glm::translate(gModelMatrix, //model matrix
				cubePositions[i] //translation vector
			);
			//roate around x axis
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

		}

		glBindVertexArray(0); //unbind the VAO

		/*********Draw LAMP START**************/
		lamp_shader.use();
		//set transformation matrices
		//pass uniforms
		lamp_shader.setMat4("u_view_matrix", gViewMatrix);
		lamp_shader.setMat4("u_perspective_matrix", gPerspectiveProjectionMatrix);
		//bind vao
		glBindVertexArray(vao_lamp);

		for(int i=0; i<_ARRAYSIZE(pointLightPositions);i++)
		{
			gModelMatrix = glm::mat4(); //set to identity
			gModelMatrix = glm::translate(gModelMatrix, pointLightPositions[i]);
			gModelMatrix = glm::scale(gModelMatrix, glm::vec3(0.2f));

			lamp_shader.setMat4("u_model_matrix", gModelMatrix);

			//draw cube
			glDrawArrays(
				GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
				0, //starting index of enabled vertex arrays
				36 //how many vertices you want to draw
			);

		} //end of draw call loop
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
	//diffuse map
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