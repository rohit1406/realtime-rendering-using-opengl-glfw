/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 5 Jan 2011

Program Name - Breakout game
*************************************************************/

//#define GLEW_STATIC
#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"

#include "game_setting_up.h"
#include "resource_manager.h"

//GLFW function declarations
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

//width of a screen
const GLuint WIN_WIDTH = 800;
const GLuint WIN_HEIGHT = 600;

Game breakout(WIN_WIDTH, WIN_HEIGHT);

int main(int argc, char *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Breakout", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError(); //from here on, all errors are of application

	glfwSetKeyCallback(window, key_callback);

	//OpenGL state configuration
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//init game
	breakout.init();

	//Deltatime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	//start game within menu state
	breakout.state = GAME_ACTIVE;

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		//calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents(); //process events that have been received

		//manage user input
		breakout.processInput(deltaTime);

		//update game state
		breakout.update(deltaTime);

		//Render
		glClearColor(0.1f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		breakout.render();

		glfwSwapBuffers(window);
	}

	//delete resources load by resource manager
	ResourceManager::clear();
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key > 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			breakout.keys[key] = GL_TRUE;
		}
		else if (action == GLFW_RELEASE)
		{
			breakout.keys[key] = GL_FALSE;
		}
	}
}