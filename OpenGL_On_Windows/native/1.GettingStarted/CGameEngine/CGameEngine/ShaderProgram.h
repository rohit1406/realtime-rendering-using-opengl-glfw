#ifndef H_SHADERPROGRAM
#define H_SHADERPROGRAM

#include<iostream>
#include<stdio.h>
#include<vector>
#include<Windows.h>

#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL

GLuint buildVertexShader(); // builds vertex shader
GLuint buildFragmentShader(); // builds fragment shader
GLuint buildShaderProgramObject(); // builds the shader program object
void startProgram(GLuint shaderProgram); // starts the shader program
void stopProgram(); // stops the shader program
#endif // !H_SHADERPROGRAM
