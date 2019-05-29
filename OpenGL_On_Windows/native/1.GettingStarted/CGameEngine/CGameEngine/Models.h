#ifndef H_MODELS
#define H_MODELS

#include<iostream>
#include<stdio.h>
#include<Windows.h>

#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL

// models
struct RawModel
{
	GLuint vaoID;
	GLuint vertexCount;
};

#endif // !H_MODELS
