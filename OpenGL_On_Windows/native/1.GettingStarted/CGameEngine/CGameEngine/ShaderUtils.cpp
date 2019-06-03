// headers
#include"ShaderUtils.h"

// set vector having 4 float values
void setVector4f(GLuint uniformLocation, GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	glUniform4f(uniformLocation, a, b, c, d);
}

// set vector having 4 float values
void setVector4v(GLuint uniformLocation, vector<GLfloat> vec)
{
	setVector4f(uniformLocation, vec[0], vec[1], vec[2], vec[3]);
}

// set vector having 3 float values
void setVector3f(GLuint uniformLocation, GLfloat a, GLfloat b, GLfloat c)
{
	glUniform3f(uniformLocation, a, b, c);
}

// set vector having 3 float values
void setVector3v(GLuint uniformLocation, vector<GLfloat> vec)
{
	setVector3f(uniformLocation, vec[0], vec[1], vec[2]);
}

// set vector having 2 float values
void setVector2f(GLuint uniformLocation, GLfloat a, GLfloat b)
{
	glUniform2f(uniformLocation, a, b);
}

// set vector having 2 float values
void setVector2v(GLuint uniformLocation, vector<GLfloat> vec) 
{
	setVector2f(uniformLocation, vec[0], vec[1]);
}

void setInt(GLuint uniformLocation, int val)
{
	glUniform1i(uniformLocation, val);
}

void setFloat(GLuint uniformLocation, GLfloat val)
{
	glUniform1f(uniformLocation, val);
}

void setBoolean(GLuint uniformLocation, bool val)
{
	glUniform1i(uniformLocation, val);
}

void setMat4(GLuint uniformLocation, glm::mat4 matrix)
{
	glUniformMatrix4fv(uniformLocation,
		1, // how many matrices you are sending 
		GL_FALSE, // do you want to transpose the matrix
		glm::value_ptr(matrix) //GLM stores their matrices not in the exact way that OpenGL likes to receive them so we first transform them with GLM's built-in function value_ptr
	);
}