/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 1 Jan 2011

Program Name - Breakout game
*************************************************************/

#include <iostream>
#include "texture.h"

Texture2D::Texture2D(): width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB), wrapS(GL_REPEAT),
					wrapT(GL_REPEAT), wrapR(GL_REPEAT), filterMin(GL_LINEAR), filterMag(GL_LINEAR)
{
	glGenTextures(1, &this->id);
}

void Texture2D::generate(GLuint width, GLuint height, unsigned char *data)
{
	this->width = width;
	this->height = height;

	//create texture
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(
		GL_TEXTURE_2D, //target
		0, //mipmap level
		this->internalFormat, //internal format
		this->width, //width
		this->height, //height
		0, //border
		this->imageFormat, //image format
		GL_UNSIGNED_BYTE, //data type
		data //actual data
	);

	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, this->wrapR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMag);

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->id);
}