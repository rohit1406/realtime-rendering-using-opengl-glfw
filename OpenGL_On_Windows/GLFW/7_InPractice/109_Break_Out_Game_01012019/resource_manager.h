/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 1 Jan 2011

Program Name - Breakout game
*************************************************************/
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <map>
#include <string>

#include "include\GL\glew.h"
#include "texture.h"
#include "shader.h"
/*
ResourceManager: A static singleton ResourceManager class that hosts several functions to load textures and shaders.
Each loadtexture or shader is store for future reference by string handles.
All functions and resources are static and no public constructor is defined.
*/
class ResourceManager
{
private:
	//private constructor
	ResourceManager(){}
	//loads and generates a shader from file
	static Shader loadShaderFromFile(const GLchar *vertexShaderFile, const GLchar *fragmentShaderFile, const GLchar *geometryShaderFile = nullptr);
	//loads a single texture from file
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
	
public:
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;

	//loads and generates a shader program from file loading vertex, fragment, geometry shaders source code
	static Shader loadShader(const GLchar *vertexShaderFile, const GLchar * fragmentShaderFile, const GLchar *geometryShaderFile,
		std::string name);

	//retrieves a stored shader
	static Shader getShader(std::string name);

	//loads and generates a texture from file
	static Texture2D loadTexture(const GLchar *file, GLboolean alpha, std::string name);

	//retrieves a stored texture
	static Texture2D getTexture(std::string name);

	//properly de-allocates all the loaded resources
	static void clear();
};
#endif // !RESOURCE_MANAGER_H
