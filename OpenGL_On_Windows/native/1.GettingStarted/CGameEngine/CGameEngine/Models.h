#ifndef H_MODELS
#define H_MODELS

// headers
#include<iostream>
#include<stdio.h>
#include<Windows.h>

#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

struct Entity
{
	struct TexturedModel texturedModel;
	glm::vec3 translate;
	float rotateX;
	float rotateY;
	float rotateZ;
	float scale; // uniform scaling
};
#endif // !H_MODELS
