// headers
#include"Loader.h"
#include"Models.h"

// Creates an empty vertex array object
GLuint createEmptyVAO()
{
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID); // create VAO
	glBindVertexArray(vaoID); // bind above vertex array object
	return vaoID;
}

// Creates VBO and stores data in it
void storeDataInBuffer(int attributeNumber, int coordinateSize, int dataSize, const float data[])
{
	GLuint vboID;
	glGenBuffers(1, &vboID); // create VBO

	//OpenGL has many types of buffer objects and the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
	//OpenGL allows us to bind to several buffers at once as long as they have a different buffer type
	glBindBuffer(GL_ARRAY_BUFFER, vboID); // bind VBO
	// pass data to VBO
	//any buffer calls we make (on the GL_ARRAY_BUFFER target) will be used to configure the 
	//currently bound buffer, which is vboID
	glBufferData(GL_ARRAY_BUFFER, // which type of buffer we want to send data to
		dataSize, // size of the data
		data, // actual data
		GL_STATIC_DRAW // the data will most likely not change at all or very rarely
	);

	// Linking vertex attributes
	// tell OpenGL how it should interpret the vertex data
	glVertexAttribPointer(
		attributeNumber, // which attribute you want to bind
		coordinateSize, // each vertex is of these many floats e.g. 3 for 3d
		GL_FLOAT, // type of data
		GL_FALSE, // want data normalized?
		coordinateSize * sizeof(GL_FLOAT), // stride
		(void *) 0 // offset in the data
	);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
}

// unbinds currently bound VAO
void unbindVAO(void)
{
	glBindVertexArray(0);
}

// loads position data to VAO
void loadPositionDataToVAO(int numElements, const float data[], int dimentsions, struct RawModel *rawModel)
{

	// create empty VAO
	rawModel->vaoID = createEmptyVAO();

	// store data in VBO
	storeDataInBuffer(0, dimentsions, numElements * sizeof(GL_FLOAT), data);

	// unbind VAO
	unbindVAO();

	rawModel->vertexCount = numElements / dimentsions;
}