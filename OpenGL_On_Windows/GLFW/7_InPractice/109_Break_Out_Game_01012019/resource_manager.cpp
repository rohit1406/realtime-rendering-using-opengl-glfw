/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 1 Jan 2011

Program Name - Breakout game
*************************************************************/
#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "include\SOIL.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::textures;
std::map<std::string, Shader>       ResourceManager::shaders;

Shader ResourceManager::loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
	shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return shaders[name];
}

Shader ResourceManager::getShader(std::string name)
{
	return shaders[name];
}

Texture2D ResourceManager::loadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	textures[name] = loadTextureFromFile(file, alpha);
	return textures[name];
}

Texture2D ResourceManager::getTexture(std::string name)
{
	return textures[name];
}

void ResourceManager::clear()
{
	//delete all shaders
	for (auto itr : shaders)
	{
		glDeleteProgram(itr.second.id);
	}

	//delete all textures
	for (auto itr : textures)
	{
		glDeleteTextures(1, &itr.second.id);
	}
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
	//retrieve the vertex, fragment, geometry shader source code from file path
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	try
	{
	std::ifstream vertexShaderFile(vShaderFile);
	std::ifstream fragmentShaderFile(fShaderFile);
	std::stringstream vShaderStream, fShaderStream;

	//read files buffer contents into stream
	vShaderStream << vertexShaderFile.rdbuf();
	fShaderStream << fragmentShaderFile.rdbuf();

	//close the file handlers
	vertexShaderFile.close();
	fragmentShaderFile.close();

	//convert stream into string
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();

	//if geometry shader is in the path
	if (gShaderFile != nullptr)
	{
		std::ifstream geometryShaderFile(gShaderFile);
		std::stringstream gShaderStream;
		gShaderStream << geometryShaderFile.rdbuf();
		geometryShaderFile.close();
		geometryCode = gShaderStream.str();
	}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: failed to read the shader files" << std::endl;
	}

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();

	//now create shader object from source code
	Shader shader;
	shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
	//create texture object
	Texture2D texture;
	if (alpha)
	{
		texture.internalFormat = GL_RGBA;
		texture.imageFormat = GL_RGBA;
	}

	int width, height;
	unsigned char *image = SOIL_load_image(file, &width, &height, 0, texture.imageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	texture.generate(width, height, image);

	//free image data
	SOIL_free_image_data(image);
	return texture;
}