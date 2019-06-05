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
#include"Models.h"
#include"MathUtils.h"
#include"Logger.h"

GLuint buildVertexShader(const char* vertexShaderFileName); // builds vertex shader
GLuint buildFragmentShader(const char* fragmentShaderFileName); // builds fragment shader
GLuint buildShaderProgramObject(const char* vertexShaderFileName, const char* fragmentShaderFileName); // builds the shader program object

#endif // !H_SHADERPROGRAM
