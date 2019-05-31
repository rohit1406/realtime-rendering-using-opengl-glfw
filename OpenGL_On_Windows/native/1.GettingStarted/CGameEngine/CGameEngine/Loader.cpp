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
void storeDataInBuffer(int attributeNumber, int coordinateSize, int numElements, const float data[])
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
		numElements * sizeof(GL_FLOAT), // size of the data
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

	// enable attribute
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
}

// stores interleaved data in the buffer
void storeInterleavedDataInBuffer(int numElements, const float data[], bool hasColorData, bool hasTexcoordData)
{
	GLuint vboID;
	GLuint stride = 3;
	GLuint offset = 0;

	//calculate stride and offset
	if (hasColorData)
	{
		stride += 3;
		offset += 3;
	}

	if (hasTexcoordData)
	{
		stride += 2;
		offset += 2;
	}

	glGenBuffers(1, &vboID); // create VBO

	//OpenGL has many types of buffer objects and the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
	//OpenGL allows us to bind to several buffers at once as long as they have a different buffer type
	glBindBuffer(GL_ARRAY_BUFFER, vboID); // bind VBO
	// pass data to VBO
	//any buffer calls we make (on the GL_ARRAY_BUFFER target) will be used to configure the 
	//currently bound buffer, which is vboID
	glBufferData(GL_ARRAY_BUFFER, // which type of buffer we want to send data to
		numElements * sizeof(GL_FLOAT), // size of the data
		data, // actual data
		GL_STATIC_DRAW // the data will most likely not change at all or very rarely
	);

	// Linking vertex attributes
	// tell OpenGL how it should interpret the vertex data
	glVertexAttribPointer(
		0, // which attribute you want to bind
		3, // each vertex is of these many floats e.g. 3 for 3d
		GL_FLOAT, // type of data
		GL_FALSE, // want data normalized?
		stride * sizeof(GL_FLOAT), // stride
		(void*)0 // offset in the data
	);

	// enable attribute
	glEnableVertexAttribArray(0);

	// if color data is present
	if(hasColorData)
	{
		// Color attribute
		glVertexAttribPointer(
			1, // which attribute you want to bind
			3, // each vertex is of these many floats e.g. 3 for 3d
			GL_FLOAT, // type of data
			GL_FALSE, // want data normalized?
			stride * sizeof(GL_FLOAT), // stride
			(void*)(offset * sizeof(GL_FLOAT)) // offset in the data
		);

		// enable attribute
		glEnableVertexAttribArray(1);
	}

	// if texcoord data is present
	if (hasTexcoordData)
	{
		// Color attribute
		glVertexAttribPointer(
			2, // which attribute you want to bind
			2, // each vertex is of these many floats e.g. 2
			GL_FLOAT, // type of data
			GL_FALSE, // want data normalized?
			stride * sizeof(GL_FLOAT), // stride
			(void*)(offset * sizeof(GL_FLOAT)) // offset in the data
		);

		// enable attribute
		glEnableVertexAttribArray(2);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
}

void loadInterleavedDataToVAO(int numElements, const float vertices[], boolean hasColorData, boolean hasTextureCoords, struct RawModel* rawModel)
{
	// create empty VAO
	rawModel->vaoID = createEmptyVAO();

	// store data in VBO
	storeInterleavedDataInBuffer(numElements, vertices, hasColorData, hasTextureCoords);

	// unbind VAO
	unbindVAO();

	int dimentions = 3;
	if (hasColorData)
	{
		dimentions += 3;
	}

	if (hasTextureCoords)
	{
		dimentions += 2;
	}
	rawModel->vertexCount = numElements / dimentions;
}


// binds the indices to be used with the VAO
void bindIndicesToVAO(int dataSize, const unsigned int indices[])
{
	GLuint eboID;
	glGenBuffers(1, &eboID); // create EBO

	//OpenGL has many types of buffer objects and the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
	//OpenGL allows us to bind to several buffers at once as long as they have a different buffer type
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID); // bind VBO
	// pass data to EBO
	//any buffer calls we make (on the GL_ARRAY_BUFFER target) will be used to configure the 
	//currently bound buffer, which is vboID
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, // which type of buffer we want to send data to
		dataSize * sizeof(GL_UNSIGNED_INT), // size of the data
		indices, // actual data
		GL_STATIC_DRAW // the data will most likely not change at all or very rarely
	);
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

// loads position data along with indices to vao
void loadPositionDataWithIndicesToVAO(int numElements, const float data[], int indicesSize, const unsigned int indices[], int dimentsions, struct RawModel* rawModel)
{
	// create empty VAO
	rawModel->vaoID = createEmptyVAO();

	// bind EBO
	/*
	A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER. 
	This also means it stores its unbind calls so make sure you don't unbind the element array buffer 
	before unbinding your VAO, otherwise it doesn't have an EBO configured. 
	*/
	bindIndicesToVAO(indicesSize, indices);

	// store data in VBO
	storeDataInBuffer(0, dimentsions, numElements, data);

	// unbind VAO
	unbindVAO();

	// unbind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	rawModel->vertexCount = numElements / dimentsions;
}