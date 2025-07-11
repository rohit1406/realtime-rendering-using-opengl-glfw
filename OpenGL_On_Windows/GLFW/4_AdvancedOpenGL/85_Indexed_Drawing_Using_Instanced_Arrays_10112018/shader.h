#ifndef SHADER_H
#define SHADER_H

#include "Include\glad\glad.h"

//OpenGL Mathematics (GLM) library
#include "Include\glm\glm.hpp"
#include "Include\glm\gtc\matrix_transform.hpp"
#include "Include\glm\gtc\type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		unsigned int geometry;

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		//check if geometry shader is there
		if (geometryPath)
		{
			//retrieve geometry shader source code from file path
			std::string geometryCode;
			std::ifstream gShaderFile;

			//ensure ifstream objects can throw exception
			gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				//open file
				gShaderFile.open(geometryPath);

				std::stringstream gShaderStream; //steam for content

												 //read file buffer contents to steam
				gShaderStream << gShaderFile.rdbuf();

				//close file handlers
				gShaderFile.close();

				//convert stream into string
				geometryCode = gShaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::GEOMETRY::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
			}

			//c_str: convert the string to array of null terminated sequence of characters and return pointer to it
			const char* gShaderCode = geometryCode.c_str();

			//compile shader
			//create geometry shader object
			geometry = glCreateShader(GL_GEOMETRY_SHADER);

			//attach source code to this shader
			glShaderSource(
				geometry, //shader
				1, //how many source codes
				&gShaderCode, //source code
				NULL
			);

			//compile
			glCompileShader(geometry);

			//check compilation errors
			checkCompileErrors(geometry, "GEOMETRY");
		}

		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);

		//if geometry shader present
		if (geometryPath)
		{
			//attach this shader to shader program object
			glAttachShader(ID, geometry);
		}
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);

		if (geometryPath)
		{
			//delete shader as it is already linked to the program object
			glDeleteShader(geometry);
		}

		glDeleteShader(fragment);
	}

	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), //name of the uniform variable
			1, //number of matrices we would like to send
			GL_FALSE, //don't transpose the matrix
			glm::value_ptr(value) //GLM stores their matrices not in the exact way that OpenGL likes to receive them so we first transform them
		);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, glm::vec3 value) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), //name of the uniform variable
			value.x,
			value.y,
			value.z
		);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), //name of the uniform variable
			x,
			y,
			z
		);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, glm::vec2 value) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), //name of the uniform variable
			value.x,
			value.y
		);
	}

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif