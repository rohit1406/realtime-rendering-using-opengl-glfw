#ifndef H_SHADER_UTILS
#define H_SHADER_UTILS

//headers
#include<iostream>
#include<vector>

#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL

using namespace std;

// uniform utilities
void setVector4f(GLuint uniformLocation, GLfloat a, GLfloat b, GLfloat c, GLfloat d);
void setVector4v(GLuint uniformLocation, vector<GLfloat> vec);
void setVector3f(GLuint uniformLocation, GLfloat a, GLfloat b, GLfloat c);
void setVector3v(GLuint uniformLocation, vector<GLfloat> vec);
void setVector2f(GLuint uniformLocation, GLfloat a, GLfloat b);
void setVector2v(GLuint uniformLocation, vector<GLfloat> vec);
void setInt(GLuint uniformLocation, int val);
void setFloat(GLuint uniformLocation, GLfloat val);
void setBoolean(GLuint uniformLocation, bool val);
#endif // !H_SHADER_UTILS
