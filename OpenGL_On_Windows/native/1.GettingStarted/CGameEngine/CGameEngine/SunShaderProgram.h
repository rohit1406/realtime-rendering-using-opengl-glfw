#ifndef H_SHADERPROGRAMSUN
#define H_SHADERPROGRAMSUN

#include<iostream>
#include<stdio.h>
#include<vector>
#include<Windows.h>

#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL

#include"ShaderUtils.h"
#include"GameUtils.h"
#include"Models.h"
#include"MathUtils.h"
#include"Logger.h"

void getAllUniformLocationsSun(); // get uniform locations

// uniforms
extern GLuint gShaderProgramSun;
//extern GLuint gLocColor;
extern GLuint gLocModelMatrixSun;
extern GLuint gLocViewMatrixSun;
extern GLuint gLocProjectionMatrixSun;

//void loadVertexColor(vector<GLfloat> color);
void loadModelMatrixSun(glm::mat4 transformationMatrix);
void loadViewMatrixSun(glm::mat4 transformationMatrix);
void loadProjectionMatrixSun(glm::mat4 transformationMatrix);
#endif // !H_SHADERPROGRAMSUN
