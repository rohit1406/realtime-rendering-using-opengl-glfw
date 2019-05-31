#ifndef H_SHADERPROGRAM
#define H_SHADERPROGRAM

#include<iostream>
#include<stdio.h>
#include<vector>
#include<Windows.h>

#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL

#include"ShaderUtils.h"
#include"GameUtils.h"

GLuint buildVertexShader(const char* vertexShaderFileName); // builds vertex shader
GLuint buildFragmentShader(const char* fragmentShaderFileName); // builds fragment shader
GLuint buildShaderProgramObject(const char* vertexShaderFileName, const char* fragmentShaderFileName); // builds the shader program object
void getAllUniformLocations(); // get uniform locations
void startProgram(GLuint shaderProgram); // starts the shader program
void stopProgram(); // stops the shader program

// uniforms
extern GLuint gShaderProgram;
extern GLuint gLocColor;

void loadVertexColor(vector<GLfloat> color);
#endif // !H_SHADERPROGRAM
