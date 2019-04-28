/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 1 Jan 2011

Program Name - Breakout game
*************************************************************/
#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include "include\GL\glew.h"
#include "include\glm\glm.hpp"
#include "include\glm\gtc\type_ptr.hpp"

#include <string>

//General purpose shader object.
//compiles from file, generate compile/link-time error messages
//hosts several utility functions for passing uniforms
class Shader
{
public:
	//state
	GLuint id;
	//constructor
	Shader() {}
	//compile shader from given source code
	void compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);
	//set's the current shader as active
	Shader &use();
	//utility functions
	void setFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
	void setInteger(const GLchar *name, GLint value, GLboolean useShader = false);
	void setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
	void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	void setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
	void setMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
private:
	//check if compilation or linking fails and if so, print the error message
	void checkCompileErrors(GLuint object, std::string type);
};
#endif // !SHADER_H
