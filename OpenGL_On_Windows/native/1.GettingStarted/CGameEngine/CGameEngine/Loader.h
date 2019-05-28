#ifndef H_LOADER
#define H_LOADER

#define GLEW_STATIC
#include<Windows.h>
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL

// function prototype declarations
GLuint createEmptyVAO(void); // Creates an empty Vertex array object
void storeDataInBuffer(int attributeNumber, int coordinateSize, int dataSize, void* data); // Creates VBO and stores data in it
void unbindVAO(void); // unbind the VAO
#endif // H_LOADER!
