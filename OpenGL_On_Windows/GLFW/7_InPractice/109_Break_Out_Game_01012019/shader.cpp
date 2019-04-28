/************************************************************
Created By - Rohit Muneshwar
E-mail - rohit.muneshwar@ymail.com
Date - 1 Jan 2011

Program Name - Breakout game
*************************************************************/

#include <iostream>
#include "shader.h"

void Shader::compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource)
{
	GLuint vertexShader, fragmentShader, geometryShader;

	//vertex shader
	//create shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//pass shader source code
	glShaderSource(
		vertexShader, //handle of the shader object
		1, //number of elements in string and length arrays
		&vertexSource, //array pointer to strings
		NULL //array of string lengths - Null because library will find the length of each zero terminated string sequence
	);
	//compile shader
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");


	//fragment shader
	//create shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//pass shader source code
	glShaderSource(
		fragmentShader, //handle of the shader object
		1, //number of elements in string and length arrays
		&fragmentSource, //array pointer to strings
		NULL //array of string lengths - Null because library will find the length of each zero terminated string sequence
	);
	//compile shader
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");

	
	//if geometry shader source code is given
	if (geometrySource != nullptr)
	{
		//geometry shader
		//create shader
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		//pass shader source code
		glShaderSource(
			geometryShader, //handle of the shader object
			1, //number of elements in string and length arrays
			&geometrySource, //array pointer to strings
			NULL //array of string lengths - Null because library will find the length of each zero terminated string sequence
		);
		//compile shader
		glCompileShader(geometryShader);
		checkCompileErrors(geometryShader, "GEOMETRY");
	}

	
	//shader program object
	//create program object
	this->id = glCreateProgram();

	//attach shaders
	glAttachShader(this->id, vertexShader);
	glAttachShader(this->id, fragmentShader);
	if (geometrySource != nullptr)
	{
		glAttachShader(this->id, geometryShader);
	}

	//link above shaders
	glLinkProgram(this->id);
	checkCompileErrors(this->id, "PROGRAM");

	//delete the shaders as they are linked into our program now and no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometrySource != nullptr)
	{
		glDeleteShader(geometryShader);
	}
}

void Shader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		//get compile status
		glGetShaderiv(
			object, //shader object
			GL_COMPILE_STATUS, //status type
			&success
			);

		if (!success) //status other than 0 means failure
		{
			glGetShaderInfoLog(
				object, //shader
				1024, //buffer size
				NULL, //length
				infoLog
			);
			std::cout << "| ERROR::SHADER: Compile time error: Type: " <<type<< std::endl;
			std::cout << infoLog << "\n---------------------------" << std::endl;
		}
	}
	else
	{
		//get link status
		glGetProgramiv(
			object, //program object
			GL_LINK_STATUS, //type
			&success //result will be stored in this
		);
		if (!success) //status other than 0 means failure
		{
			glGetProgramInfoLog(
				object, //program object
				1024, //buffer size
				NULL, //length
				infoLog
			);
			std::cout << "| ERROR::SHADER: Link time error: Type: " << type << std::endl;
			std::cout << infoLog << "\n---------------------------" << std::endl;
		}
	}
}

Shader &Shader::use()
{
	glUseProgram(this->id);
	return *this;
}


//utility functions
void Shader::setFloat(const GLchar *name, GLfloat value, GLboolean useShader)
{
	if (useShader)
	{
		this->use();
	}
	GLint uLocation = glGetUniformLocation(this->id, name);
	glUniform1f(uLocation, value);
}

void Shader::setInteger(const GLchar *name, GLint value, GLboolean useShader)
{
	if (useShader)
	{
		this->use();
	}
	GLint uLocation = glGetUniformLocation(this->id, name);
	glUniform1i(uLocation, value);
}

void Shader::setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
	{
		this->use();
	}
	GLint uLocation = glGetUniformLocation(this->id, name);
	glUniform2f(uLocation, x, y);
}

void Shader::setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader)
{
	if (useShader)
	{
		this->use();
	}
	GLint uLocation = glGetUniformLocation(this->id, name);
	glUniform2f(uLocation, value.x, value.y);
}

void Shader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
	{
		this->use();
	}
	GLint uLocation = glGetUniformLocation(this->id, name);
	glUniform3f(uLocation, x, y, z);
}

void Shader::setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader)
{
	if (useShader)
	{
		this->use();
	}
	GLint uLocation = glGetUniformLocation(this->id, name);
	glUniform3f(uLocation, value.x, value.y, value.z);
}

void Shader::setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
	{
		this->use();
	}
	GLint uLocation = glGetUniformLocation(this->id, name);
	glUniform4f(uLocation, x, y, z, w);
}

void Shader::setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader)
{
	if (useShader)
	{
		this->use();
	}
	GLint uLocation = glGetUniformLocation(this->id, name);
	glUniform4f(uLocation, value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader)
{
	if (useShader)
	{
		this->use();
	}
	GLint uLocation = glGetUniformLocation(this->id, name);
	glUniformMatrix4fv(uLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}