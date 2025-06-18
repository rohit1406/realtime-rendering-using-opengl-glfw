/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 18 Oct 2018

	Program Name - Two Triangles Next To Each Other using OpenGL

	Important Notes:
	We can register the callback functions after we have created the window and before gameloop is initiated
*/

//header files
#include<iostream>
#include "glad\glad.h" //glad.h should be before glfw3.h; it contains correct OpenGL headers includes
#include "GLFW\glfw3.h"

//macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define VIEWPORT_WIDTH 800
#define VIEWPORT_HEIGHT 600

//global variable declaration
const char* vertexShaderSource =
"#version 330 core"\
"\n"\
"layout (location = 0) in vec3 aPos;"\
"void main()"\
"{"\
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"\
"}";

const char* fragmentShaderSource =
"#version 330 core"\
"\n"\
"out vec4 FragColor;"\
"void main()"\
"{"\
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"\
"}";
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
	
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //this is neede for MAC only

	// Create Window Object
	// ---------------------
	GLFWwindow* window = glfwCreateWindow(
									WIN_WIDTH, //width
									WIN_HEIGHT, //height
									"OpenGL", //title
									NULL, //monitor
									NULL //share
								);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); //terminate the program
		return -1;
	}

	//make context of our window the main context on the current thread
	glfwMakeContextCurrent(window);

	// Initialize GLAD - load all OpenGL function pointers
	// ----------------------------------------------------
	//we pass GLAD the function to load the address of the OpenGL function pointers which is OS specific
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/*****************Build And Compile Our Shader Program Starts********************/
	//VERTEX SHADER COMPILATION
	//create memory on GPU - Vertex Shader Object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //creating vertex shader object

	//attach vertex shader source code to shader object
	glShaderSource(
					vertexShader, //to which shader object you want to attach
					1, //how many strings you are passing as a source code
					&vertexShaderSource, //actual source code of vertex shader
					NULL
				);

	//compile shader source code
	glCompileShader(vertexShader);

	//check status of the compilation
	int success;
	char infoLog[512];
	//get compilation status
	glGetShaderiv(
				vertexShader, //which shader object
				GL_COMPILE_STATUS, //what is compilation status of my shader object
				&success //store the result in this
				);

	if (!success)
	{
		//get logs
		glGetShaderInfoLog(
			vertexShader, //from which shader object you want to get logs
			512, //buffer size
			NULL,
			infoLog //buffer
				);

		std::cout << "ERROR: Vertx Shader Logs: COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//FRAGMENT SHADER COMPILATION
	//create memory on GPU - Fragment Shader Object
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //creating fragment shader object

													 //attach vertex shader source code to shader object
	glShaderSource(
		fragmentShader, //to which shader object you want to attach
		1, //how many strings you are passing as a source code
		&fragmentShaderSource, //actual source code of fragment shader
		NULL
	);

	//compile shader source code
	glCompileShader(fragmentShader);

	//check status of the compilation
	//re-initializing variables
	success = 0;
	//infoLog = NULL;
	//get compilation status
	glGetShaderiv(
		fragmentShader, //which shader object
		GL_COMPILE_STATUS, //what is compilation status of my shader object
		&success //store the result in this
	);

	if (!success)
	{
		//get logs
		glGetShaderInfoLog(
			fragmentShader, //from which shader object you want to get logs
			512, //buffer size
			NULL,
			infoLog //buffer
		);

		std::cout << "ERROR: Fragment Shader Logs: COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//SHADER PROGRAM OBJECT
	//create memory on GPU - Shader Program Object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); //creating shader program object

	//attach vertex shader object to shader program object
	glAttachShader(
		shaderProgram, //which program object
		vertexShader //attach vertex shader
	);

	//attach fragment shader object to shader program object
	glAttachShader(
		shaderProgram, //which program object
		fragmentShader //attach fragment shader
	);

	//link output of prev. shader to input of next shader
	glLinkProgram(shaderProgram);

	//check status of the linking
	//re-initializing variables
	success = 0;
	//infoLog = NULL;
	//get link status
	glGetProgramiv(
		shaderProgram, //which shader program object
		GL_LINK_STATUS, //what is link status of my shader object
		&success //store the result in this
	);

	if (!success)
	{
		//get logs
		glGetProgramInfoLog(
			shaderProgram, //from which shader program object you want to get logs
			512, //buffer size
			NULL,
			infoLog //buffer
		);

		std::cout << "ERROR: Shader Program Object Logs: LINKAGE_FAILED\n" << infoLog << std::endl;
	}

	//delete shader objects now, as we no longer require it
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/*****************Build And Compile Our Shader Program Ends********************/

	//Input Vertex Data
	float vertices[] =
	{
		//position
		//first triangle
		-0.8f, -0.5f, 0.0f, //bottom left
		0.0f, -0.5f, 0.0f, //bottom right
		-0.4f, 0.5f, 0.0f,

		//second triangle
		0.0f, -0.5f, 0.0f,
		0.8f, -0.5f, 0.0f,
		0.4f, 0.5f, 0.0f

	};

	//create vertex array object
	//a VAO that stores our vertex attribute configuration
	//and which VBO to use
	unsigned int VAO;
	glGenVertexArrays(1,  //how many vertex array objects you want to create
					&VAO //id of vertex array object is store in this
				);
	//bind vertex array object first, then bind and set vertex buffers
	//and then configure vertex attributes
	glBindVertexArray(VAO);

	//create memory on GPU - vertex buffer object
	unsigned int VBO;
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
	glVertexAttribPointer(
					0, //which vertex attribute we want to configure
					3, //size of the vertex attribute - 3 float values for each vertex
					GL_FLOAT, //type of the data
					GL_FALSE, //do you want the data to be normalized (in -1 to 1 range)
					3 * sizeof(float), //space between consecutive vertex attribtes - stride
					(void *) 0 //from where the position data begins in the buffer
				);

	glEnableVertexAttribArray(0); //enable vertex attribute; 0 is vertex attribute location
		
	//this is allowed; the call to glVertexAttribPointer registerd VBO as the vertex attribute's bound vertex buffer object;
	//so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vertex buffer

	//you can unbind the vao afterwards so that other VAO calls won't accidentaly modify this VAO
	glBindVertexArray(0);

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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		//clear the screens color buffer
		//here we pass in buffer bits to specify which buffer we would like to clear
		glClear(GL_COLOR_BUFFER_BIT);


		//draw our first triangle
		//activate shader program object
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//draw triangle
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

	//de-allocate all resources once they have outlived their purposes
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
}