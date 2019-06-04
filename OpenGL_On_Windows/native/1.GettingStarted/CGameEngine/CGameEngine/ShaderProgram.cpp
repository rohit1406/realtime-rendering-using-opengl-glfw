#include"ShaderProgram.h"

using namespace std;

GLuint gShaderProgram;
GLuint gLocColor;
GLuint gLocPositionOffset;
GLuint gLocBricksSampler;
GLuint gLocFaceSampler;
GLuint gLocMixParam;
GLuint gLocModelMatrix;
GLuint gLocViewMatrix;
GLuint gLocProjectionMatrix;

// builds the vertex shader
GLuint buildVertexShader(const char *vertexShaderFileName)
{
	// local variables
	GLuint vertexShader;

	// shader source code
	std::string vertexShaderSourceCode = readShader(vertexShaderFileName);
	const char* vsCode = vertexShaderSourceCode.c_str();
	// code
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // creates vertex shader object
	gShaderList.push_back(vertexShader); // add shader to the list of shaders
	// attache source code to the above shader
	glShaderSource(vertexShader, 1, &vsCode, NULL);
	// compile the shader
	glCompileShader(vertexShader);

	// check for compilation errors if any
	int success; // result of the compilations
	char infoLog[512]; // logs buffer
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // check compile status
	// check the status
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // fetch the logs
		//print the logs
		logStaticData("ERROR::COMPILATION_FAILURE::Vertex Shader Logs: ");
		logStaticData(infoLog);
	}
	return vertexShader;
}

// builds fragment shader
GLuint buildFragmentShader(const char* fragmentShaderFileName)
{
	// local variables
	GLuint fragmentShader;

	// shader source code
	std::string fragmentShaderSourceCode = readShader(fragmentShaderFileName);
	const char* fsCode = fragmentShaderSourceCode.c_str();
	// code
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // creates fragment shader object
	gShaderList.push_back(fragmentShader); // add shader to the list of shaders
	// attache source code to the above shader
	glShaderSource(fragmentShader, 1, &fsCode, NULL);
	// compile the shader
	glCompileShader(fragmentShader);

	// check for compilation errors if any
	int success; // result of the compilations
	char infoLog[512]; // logs buffer
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // check compile status
	// check the status
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); // fetch the logs
		//print the logs
		logStaticData("ERROR::COMPILATION_FAILURE::Fragment Shader Logs: ");
		logStaticData(infoLog);
	}
	return fragmentShader;
}

// builds the shader program object
GLuint buildShaderProgramObject(const char* vertexShaderFileName, const char* fragmentShaderFileName)
{
	// local variables
	GLuint shaderProgramObject;

	// code
	// get the vertex shader and fragment shader objects
	GLuint vertexShader = buildVertexShader(vertexShaderFileName);
	GLuint fragmentShader = buildFragmentShader(fragmentShaderFileName);
	// create shader program
	shaderProgramObject = glCreateProgram();
	gShaderProgramObjectList.push_back(shaderProgramObject);
	// attach shaders to the shader program object
	glAttachShader(shaderProgramObject, vertexShader);
	glAttachShader(shaderProgramObject, fragmentShader);
	// link vertex and fragment shaders
	glLinkProgram(shaderProgramObject);

	// check if linkage error occures
	// check for compilation errors if any
	int success; // result of the compilations
	char infoLog[512]; // logs buffer
	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &success); // check compile status
	// check the status
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramObject, 512, NULL, infoLog); // fetch the logs
		//print the logs
		logStaticData("ERROR::LINKAGE_FAILURE::Linkage Shader Logs: ");
		logStaticData(infoLog);
	}
	
	gShaderProgram = shaderProgramObject;
	return shaderProgramObject;
}

// starts the shader program
void startProgram(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);
}

// stops the shader program
void stopProgram()
{
	glUseProgram(0);
}


// gets all uniform locations
void getAllUniformLocations()
{
	gLocColor = glGetUniformLocation(gShaderProgram, "u_vertex_color");
	gLocPositionOffset = glGetUniformLocation(gShaderProgram, "u_position_offset");
	gLocBricksSampler = glGetUniformLocation(gShaderProgram, "u_bricks_sampler");
	gLocFaceSampler = glGetUniformLocation(gShaderProgram, "u_face_sampler");
	gLocMixParam = glGetUniformLocation(gShaderProgram, "u_mix_texture_param");
	gLocModelMatrix = glGetUniformLocation(gShaderProgram, "u_model_matrix");
	gLocViewMatrix = glGetUniformLocation(gShaderProgram, "u_view_matrix");
	gLocProjectionMatrix = glGetUniformLocation(gShaderProgram, "u_projection_matrix");
}

// loads vertex color
void loadVertexColor(vector<GLfloat> color)
{
	setVector4v(gLocColor, color);
}

// set the position offset to the triangle
void loadPositionOffset(float offset)
{
	setFloat(gLocPositionOffset, offset);
}

// bind texture units
void bindTextureUnits(struct TexturedModel texturedModel)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturedModel.textureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturedModel.textureIDFace);
}

void loadBricksTextureSampler(GLuint samplerID)
{
	setInt(gLocBricksSampler, samplerID);
}

void loadFaceTextureSampler(GLuint samplerID)
{
	setInt(gLocFaceSampler, samplerID);
}

void loadMixParam(GLfloat value)
{
	setFloat(gLocMixParam, value);
}

void loadModelMatrix(glm::mat4 transformationMatrix)
{
	setMat4(gLocModelMatrix, transformationMatrix);
}

void loadViewMatrix(glm::mat4 transformationMatrix)
{
	setMat4(gLocViewMatrix, transformationMatrix);
}

void loadProjectionMatrix(glm::mat4 transformationMatrix)
{
	setMat4(gLocProjectionMatrix, transformationMatrix);
}