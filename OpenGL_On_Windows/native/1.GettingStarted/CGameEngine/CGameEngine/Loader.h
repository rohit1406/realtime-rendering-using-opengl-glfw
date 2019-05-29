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
extern void loadPositionDataToVAO(int dataSize, const float data[], int dimentsions, struct RawModel *rawModel); // loads position data to vao
#endif // H_LOADER!
