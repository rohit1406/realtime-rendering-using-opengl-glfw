#ifndef H_MODELS
#define H_MODELS

// headers
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

struct TexturedModel
{
	struct RawModel rawModel;
	GLuint textureID;
	GLuint textureIDFace;
};
#endif // !H_MODELS
