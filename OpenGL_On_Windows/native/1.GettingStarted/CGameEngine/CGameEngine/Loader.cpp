// headers
#include"Loader.h"
#include"Models.h"
// for image loading
// the preprocessor modifies the header file such that it only contains 
// the relevant definition source code, effectively turning the header file into a .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

// function prototype declarations
void storeDataInBuffer(int attributeNumber, int coordinateSize, int dataSize, const float data[]); // Creates VBO and stores data in it
void storeInterleavedDataInBuffer(int numElements, const float data[], bool hasColorData, bool hasTexcoordData);
void storeInterleavedDataPosColNorTexInBuffer(int numElements, const float data[], bool hasColorData, bool hasNormalData, bool hasTexcoordData);


// Creates an empty vertex array object
GLuint createEmptyVAO()
{
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID); // create VAO
	glBindVertexArray(vaoID); // bind above vertex array object

	// add vaoID to list of VAOs
	gVAOList.push_back(vaoID);
	return vaoID;
}

// Creates VBO and stores data in it
void storeDataInBuffer(int attributeNumber, int coordinateSize, int numElements, const float data[])
{
	GLuint vboID;
	glGenBuffers(1, &vboID); // create VBO
	gVBOList.push_back(vboID); // add VBO to the list of VBOs
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

	if(hasColorData)
		stride += 3;
	if (hasTexcoordData)
	{
		stride += 2;
	}
	glGenBuffers(1, &vboID); // create VBO
	gVBOList.push_back(vboID); // add VBO to the list of VBOs
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
		offset += 3;

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
		offset += 3;

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

// stores interleaved data in the buffer
void storeInterleavedDataPosColNorTexInBuffer(int numElements, const float data[], bool hasColorData, bool hasNormalData, bool hasTexcoordData)
{
	GLuint vboID;
	GLuint stride = 3;
	GLuint offset = 0;

	if (hasColorData)
		stride += 3;
	if (hasTexcoordData)
	{
		stride += 2;
	}
	if (hasNormalData)
	{
		stride += 3;
	}

	glGenBuffers(1, &vboID); // create VBO
	gVBOList.push_back(vboID); // add VBO to the list of VBOs
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
	if (hasColorData)
	{
		offset += 3;

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

	// if normal data is present
	if (hasNormalData)
	{
		offset += 3;

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
		offset += 3;

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
	gVBOList.push_back(eboID); // add EBO to the list of VBOs
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

// // loads interleaved data with indices to VAO
void loadInterleavedDataWithIndicesToVAO(int numElements, const float vertices[], boolean hasColorData, boolean hasTextureCoords, int indicesSize, const unsigned int indices[], struct RawModel* rawModel)
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
	storeInterleavedDataInBuffer(numElements, vertices, hasColorData, hasTextureCoords);

	// unbind VAO
	unbindVAO();

	// unbind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	rawModel->vertexCount = indicesSize;
}

// loads the texture to the texture buffer and returns it's id
GLuint loadTexture(const std::string fileName)
{
	// local variables
	GLuint textureID;
	int width, height, nrChannels;

	// code
	// Create the texture buffer
	glGenTextures(1, &textureID);
	gTexturesList.push_back(textureID); // add texture to the list of textures
	// bind the buffer
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
	OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, 
	but images usually have 0.0 at the top of the y-axis. Luckily for us, stb_image.h 
	can flip the y-axis during image loading by adding the following statment before loading 
	any image.
	*/
	stbi_set_flip_vertically_on_load(true);
	// first load the image data to the memory
	unsigned char* data = stbi_load(
		fileName.c_str(), // file to be loaded as a texture
		&width, // width of the image
		&height, // height of the image
		&nrChannels, // number of channels of the image
		0
	);

	GLenum format = GL_RGB;
	if (nrChannels == 3) // jpg
	{
		format = GL_RGB;
	}
	else if (nrChannels == 4) // png
	{
		format = GL_RGBA;
	}

	if(data)
	{
	// pass data to the buffer
	glTexImage2D(
		GL_TEXTURE_2D, // texture target
		0,  // mipmap level
		format, // internal texture format
		width, // width of the texture
		height, // height
		0, // border
		format, // data format
		GL_UNSIGNED_BYTE, // type of data
		data // actual data
	);

	// generate texture mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Error while loading the image.\n");
	}
	// free image data now, as we have already passed it to buffer
	stbi_image_free(data);

	return textureID;
}

void loadInterleavedDataPosColNorTexToVAO(int numElements, const float vertices[], boolean hasColorData, boolean hasNormals, boolean hasTextureCoords, struct RawModel* rawModel)
{
	// create empty VAO
	rawModel->vaoID = createEmptyVAO();

	// store data in VBO
	storeInterleavedDataPosColNorTexInBuffer(numElements, vertices, hasColorData, hasNormals, hasTextureCoords);

	// unbind VAO
	unbindVAO();

	int dimentions = 3;
	if (hasColorData)
	{
		dimentions += 3;
	}

	if (hasNormals)
	{
		dimentions += 3;
	}

	if (hasTextureCoords)
	{
		dimentions += 2;
	}
	rawModel->vertexCount = numElements / dimentions;
}