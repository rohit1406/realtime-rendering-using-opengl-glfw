#ifndef H_STATE_CHANGER
#define H_STATE_CHANGER

// headers
#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL
#include"GameUtils.h"
#include"Logger.h"

void startProgram(GLuint shaderProgram); // starts the shader program
void stopProgram(); // stops the shader program
void enableWireframeMode(); // enables wireframe mode
void disableWireframeMode(); // disables wireframe mode
void enableDepthTesting(); // enables depth testing
void disableDepthTesting(); // disable depth testing
#endif // !H_STATE_CHANGER
