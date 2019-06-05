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

// materials properties of an objects
struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

// textured models having it's own translations, rotations and scaling factors
struct Entity
{
	struct TexturedModel texturedModel;
	glm::vec3 translate;
	float rotateX;
	float rotateY;
	float rotateZ;
	float scale; // uniform scaling
	struct Material material;
};

// light source - as light is also one of the entity
struct Light
{
	struct Entity entity;
	glm::vec3 lightPosition;
	glm::vec3 lightColor;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
#endif // !H_MODELS
