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

// function prototype declarations
GLuint createEmptyVAO(void); // Creates an empty Vertex array object
void storeDataInBuffer(int attributeNumber, int coordinateSize, int dataSize, const float data[]); // Creates VBO and stores data in it
void unbindVAO(void); // unbind the VAO
extern void loadPositionDataToVAO(int numElements, const float data[], int dimentsions, struct RawModel *rawModel); // loads position data to vao
extern void loadPositionDataWithIndicesToVAO(int numElements, const float data[], int indicesSize, const unsigned int indices[], int dimentsions, struct RawModel* rawModel); // loads position data along with indices to vao
void bindIndicesToVAO(int dataSize, const unsigned int indices[]); // binds the indices to be used with the VAO
#endif // H_LOADER!
