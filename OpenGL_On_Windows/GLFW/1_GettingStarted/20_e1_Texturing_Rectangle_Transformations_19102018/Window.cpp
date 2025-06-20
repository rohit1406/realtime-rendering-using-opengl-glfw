/*
	Created By - Rohit Muneshwar
	E-mail - rohit.muneshwar@ymail.com
	Date - 19 Oct 2018

	Program Name - Draw Indexed Rectangle along with Texture with Applying Transformations
	First Scale then translate then rotate

	Important Notes:
	We can register the callback functions after we have created the window and before gameloop is initiated
*/

//header files
#include<iostream>
#include "glad\glad.h" //glad.h should be before glfw3.h; it contains correct OpenGL headers includes
#include "GLFW\glfw3.h"

//OpenGL Mathematics (GLM) library
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

//to manage shaders and program objects
#include "shader.h"

//by defining this, the preprocessor modifies the header file such that it contains only the relevant definition source code
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define VIEWPORT_WIDTH 800
#define VIEWPORT_HEIGHT 600

//global variable declaration
float counter = 0.2f;

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
									"OpenGL: Rectangle", //title
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
	Shader ourShader("vertex_shader.vs", "fragment_shader.fs");
	/*****************Build And Compile Our Shader Program Ends********************/

	//Input Vertex Data
	float vertices[] =
	{
		//position			//Color				//tex coords
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, //top-right
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, //bottom-right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, //bottom-left
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f //top-left
	};

	//indices
	unsigned int indices[] =
	{
		0, 1, 3, //first triangle
		1, 2, 3 //second triangle
	};

	//create vertex array object
	//a VAO that stores our vertex attribute configuration
	//and which VBO to use
	unsigned int VAO, VBO, EBO;
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

	//create element buffer object
	glGenBuffers(1,  //how many buffers
				&EBO //this will contain id of the buffer object
				);

	//bind above buffer to element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, //I want to bind to the element buffer
				EBO //buffer which will get bind to element buffer object
				);

	//copy indices to this buffers memory
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, //this type of buffer is your target, which is EBO
		sizeof(indices), //size of the data which we will pass
		indices, //actual data
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

	//color data
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
		(void *)(2 * 3 * sizeof(float)) //from where the position data begins in the buffer
	);

	glEnableVertexAttribArray(2); //enable vertex attribute; 0 is vertex attribute location
		
	//this is allowed; the call to glVertexAttribPointer registerd VBO as the vertex attribute's bound vertex buffer object;
	//so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vertex buffer

	//you can unbind the vao afterwards so that other VAO calls won't accidentaly modify this VAO
	glBindVertexArray(0);

	/**************LOAD and CREATE TEXTURE*******************/
	//BOX texture
	unsigned int texture;
	//generate texture
	glGenTextures(1, //count
				&texture //id of the created texture will be stored here
			);

	//bind the created texture to the texture type
	//all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	glBindTexture(GL_TEXTURE_2D, texture);

	//set the texture wraping parameters
	glTexParameteri(GL_TEXTURE_2D, //target
					GL_TEXTURE_WRAP_S, //WRAP operation for S texture axis
					GL_REPEAT //this is default wraping method
					);

	glTexParameteri(GL_TEXTURE_2D, //target
					GL_TEXTURE_WRAP_T, //WRAP operation for T texture axis
					GL_REPEAT //this is default wraping method
				);

	//set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, //target
		GL_TEXTURE_MIN_FILTER, //when scaling down
		GL_LINEAR //takes interpolated value from the tex coordinate's neighbouring texels
	);

	glTexParameteri(GL_TEXTURE_2D, //target
		GL_TEXTURE_MAG_FILTER, //when scaling up
		GL_LINEAR //takes interpolated value from the tex coordinate's neighbouring texels
	);

	/****load image, create texture and generate mipmaps****/
	int width, height, nrChannels;

	//load image data
	unsigned char* data = stbi_load(
							"container.jpg", //file name						
							&width, //width of the file
							&height, //height of the file
							&nrChannels, //number of colored channels
							0
						);

	//check if image data loaded successfully
	if (data)
	{
		//generate texture from this texture data
		glTexImage2D(
			GL_TEXTURE_2D, //target; this operation will generate texture on currently bound texture object
			0, //base mipmap level
			GL_RGB, //internal format; in what kind of format we want to store the texture
			width, //width of resulting texture
			height, //height of resulting texture
			0, //this should be always 0, for legacy purpose
			GL_RGB, //format of source image
			GL_UNSIGNED_BYTE, //we have stored the image data as GL_UNSIGNED_BYTE
			data //actual data
		);

		//now the currently bound texture object has the texture image attached to it
		//but it only has the base-level of the texture image loaded
		//below will automatically generate all the required mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::LOADING_IMAGE_FAILED::BOX" << std::endl;
	}

	//free image data now
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

	//Smiley texture
	unsigned int texture_smiley;
	//generate texture
	glGenTextures(1, //count
		&texture_smiley //id of the created texture will be stored here
	);

	//bind the created texture to the texture type
	//all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	glBindTexture(GL_TEXTURE_2D, texture_smiley);

	//set the texture wraping parameters
	glTexParameteri(GL_TEXTURE_2D, //target
		GL_TEXTURE_WRAP_S, //WRAP operation for S texture axis
		GL_REPEAT //this is default wraping method
	);

	glTexParameteri(GL_TEXTURE_2D, //target
		GL_TEXTURE_WRAP_T, //WRAP operation for T texture axis
		GL_REPEAT //this is default wraping method
	);

	//set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, //target
		GL_TEXTURE_MIN_FILTER, //when scaling down
		GL_LINEAR //takes interpolated value from the tex coordinate's neighbouring texels
	);

	glTexParameteri(GL_TEXTURE_2D, //target
		GL_TEXTURE_MAG_FILTER, //when scaling up
		GL_LINEAR //takes interpolated value from the tex coordinate's neighbouring texels
	);

	//images usually have 0.0 at the top of y-axis, but OpenGL expects it at the bottom of the image
	stbi_set_flip_vertically_on_load(true);

	/****load image, create texture and generate mipmaps****/
	//re-initialize variables
	width = height = nrChannels = 0;
	//free(data);

	//load image data
	data = stbi_load(
		"awesomeface.png", //file name						
		&width, //width of the file
		&height, //height of the file
		&nrChannels, //number of colored channels
		0
	);

	//check if image data loaded successfully
	if (data)
	{
		//generate texture from this texture data
		glTexImage2D(
			GL_TEXTURE_2D, //target; this operation will generate texture on currently bound texture object
			0, //base mipmap level
			GL_RGB, //internal format; in what kind of format we want to store the texture
			width, //width of resulting texture
			height, //height of resulting texture
			0, //this should be always 0, for legacy purpose
			GL_RGBA, //format of source image; since png format has alpha(transperancy) channel also
			GL_UNSIGNED_BYTE, //we have stored the image data as GL_UNSIGNED_BYTE
			data //actual data
		);

		//now the currently bound texture object has the texture image attached to it
		//but it only has the base-level of the texture image loaded
		//below will automatically generate all the required mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::LOADING_IMAGE_FAILED::SMIELY" << std::endl;
	}

	//free image data now
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

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
		ourShader.use();
		ourShader.setInt("u_texture_sampler_box", 0);
		ourShader.setInt("u_texture_sampler_smiley", 1);
		ourShader.setFloat("opacity",counter);

		//set window title bar
		//char title[20];
		//wsprintf(title, "OpenGL: Texture Opacity - %i",counter);
		//glfwSetWindowTitle(window, title);
		//bind which VAO we are going to use with the program object
		glBindVertexArray(VAO);

		//Activate Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		//bind the texture now
		glBindTexture(GL_TEXTURE_2D, texture);

		//Activate Texture Unit 1
		glActiveTexture(GL_TEXTURE1);
		//bind the texture now
		glBindTexture(GL_TEXTURE_2D, texture_smiley);

		/*********CREATE TRANSFORMATION MATRICES********/
		glm::mat4 transformationMatrix; //identity transformation matrix
		//rotate
		transformationMatrix = glm::rotate(transformationMatrix, //transformation matrix
			(float)glfwGetTime(),
			glm::vec3(0.0f, 0.0f, 1.0f) //rotate around z axis
		);

		//translate
		transformationMatrix = glm::translate(transformationMatrix, //transformation matrix
										glm::vec3(0.5f, -0.5f, 0.0f) //translation vector
									);
		
		
		//scale
		transformationMatrix = glm::scale(transformationMatrix, //transformation matrix
									glm::vec3(0.5f, 0.5f, 0.5f) //scale uniformly across all axis by 0.5
								);

		//pass transformationMatrix to the shader
		ourShader.setMat4("transform", transformationMatrix);

		//draw rectangle
		/*glDrawArrays(
				GL_TRIANGLES, //what we want to draw using the vertices given in the shader program object
				0, //starting index of enabled vertex arrays
				3 //how many vertices you want to draw
			);*/
		glDrawElements(
				GL_TRIANGLES, //what you want to draw
				6, //count of the vertices we want to draw, as 6 indices are provided
				GL_UNSIGNED_INT,  //type of indices
				0 //offset in EBO
			);

		glBindVertexArray(0); //unbind the VAO
		
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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //if up arrow key is pressed
	{
		if (counter >= 1.0f)
		{
			counter = 1.0f;
		}
		else
		{
			counter = counter + 0.0001f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //if down arrow key is pressed
	{
		if (counter <= 0.0f)
		{
			counter = 0.0f;
		}
		else
		{
			counter = counter - 0.0001f;
		}
	}
}