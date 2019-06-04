#ifndef H_LOADER
#define H_LOADER

// headers


#include<iostream>
#include<stdio.h>
#include<vector>
#include<Windows.h>



#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL
#include"Logger.h"
#include"GameUtils.h"


// function prototype declarations
GLuint createEmptyVAO(void); // Creates an empty Vertex array object
void unbindVAO(void); // unbind the VAO
extern void loadPositionDataToVAO(int numElements, const float data[], int dimentsions, struct RawModel *rawModel); // loads position data to vao
extern void loadPositionDataWithIndicesToVAO(int numElements, const float data[], int indicesSize, const unsigned int indices[], int dimentsions, struct RawModel* rawModel); // loads position data along with indices to vao
void bindIndicesToVAO(int dataSize, const unsigned int indices[]); // binds the indices to be used with the VAO
void loadInterleavedDataToVAO(int numElements, const float vertices[], boolean hasColorData, boolean hasTextureCoords, struct RawModel* rawModel); // loads interleaved data to VAO
GLuint loadTexture(const std::string fileName); // loads texture and returns texture buffer id
void loadInterleavedDataWithIndicesToVAO(int numElements, const float vertices[], boolean hasColorData, boolean hasTextureCoords, int indicesSize, const unsigned int indices[], struct RawModel* rawModel); // loads interleaved data with indices to VAO
#endif // H_LOADER!
