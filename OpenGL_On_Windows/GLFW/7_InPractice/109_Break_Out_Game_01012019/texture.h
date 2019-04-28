/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 1 Jan 2011

Program Name - Breakout game
*************************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include "include\GL\glew.h"

//Texture2D: able to store and configure 2D texture in OpenGL
//It also hosts utility functions for easy managements
class Texture2D
{
public:
	//state
	GLuint id;
	//texture image dimensions
	GLuint width, height;
	//texture formats
	GLuint internalFormat;
	GLuint imageFormat;
	//texture configuration
	GLuint wrapS;
	GLuint wrapT;
	GLuint wrapR;
	GLuint filterMin;
	GLuint filterMag;

	//constructor
	Texture2D();
	//Generates texture from image data
	void generate(GLuint width, GLuint height, unsigned char *data);
	//binds the texture as the current active GL_TEXTURE_2D texture object
	void bind() const;
};
#endif // !TEXTURE_H
