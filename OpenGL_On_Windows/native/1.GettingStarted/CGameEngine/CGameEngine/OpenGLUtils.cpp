// headers
#include"OpenGLUtils.h"

// enables wireframe mode
// wireframe mode shows that the model indeed consists of a triangles.
// It configures how OpenGL draws it's primitives
void enableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	logStaticData("Enabled wireframe mode");
}

// disables wireframe mode
void disableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	logStaticData("Disabled wireframe mode");
}

// enables depth testing
void enableDepthTesting()
{
	glEnable(GL_DEPTH_TEST);
	logStaticData("Enabled depth testing");
}

// disable depth testing
void disableDepthTesting()
{
	glDisable(GL_DEPTH_TEST);
	logStaticData("Disabled depth testing");
}

// starts the shader program
void startProgram(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);
}

// stops the shader program
void stopProgram()
{
	glUseProgram(0);
}