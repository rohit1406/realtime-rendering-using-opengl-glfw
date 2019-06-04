#ifndef H_GAME_UTILS
#define H_GAME_UTILS

// headers
#include<Windows.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL
#include<iostream>
#include<vector>

// for image loading
// the preprocessor modifies the header file such that it only contains 
// the relevant definition source code, effectively turning the header file into a .cpp file
//#define STB_IMAGE_IMPLEMENTATION
//#include"stb_image.h"

extern std::string SHADER_RESOURCE_FILE_LOC;
extern std::string IMAGES_RESOURCE_FILE_LOC;
extern std::vector<GLuint> gShaderList;
extern std::vector<GLuint> gShaderProgramObjectList;
extern std::vector<GLuint> gVBOList;
extern std::vector<GLuint> gVAOList;
extern std::vector<GLuint> gTexturesList;

std::string readShader(const char* fileName); // read the shader file
void draw(GLuint vertexCount, bool isIndexed);

extern long openGLInitializationTime; // time at which OpenGL is initialized
uint64_t timeSinceEpochMillisec();
long getTimeInSecondsSinceOpenGLIsInitialized();
#endif // !H_GAME_UTILS
