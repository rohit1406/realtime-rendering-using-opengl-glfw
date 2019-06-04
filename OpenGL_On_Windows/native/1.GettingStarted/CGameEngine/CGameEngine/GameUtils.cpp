// headers
#include"GameUtils.h"

// global variables
std::string SHADER_RESOURCE_FILE_LOC = "C:\\Users\\RohitMuneshwar\\Documents\\IBM\\RTR\\projects\\learn_opengl\\OpenGL_On_Windows\\native\\1.GettingStarted\\CGameEngine\\CGameEngine\\res\\shaders\\";
std::string IMAGES_RESOURCE_FILE_LOC = "C:\\Users\\RohitMuneshwar\\Documents\\IBM\\RTR\\projects\\learn_opengl\\OpenGL_On_Windows\\native\\1.GettingStarted\\CGameEngine\\CGameEngine\\res\\images\\";
long openGLInitializationTime = 0.0f; // time at which OpenGL is initialized
FILE* gLogfile = NULL; // log file

// for clean up activities
std::vector<GLuint> gShaderList;
std::vector<GLuint> gShaderProgramObjectList;
std::vector<GLuint> gVBOList;
std::vector<GLuint> gVAOList;
std::vector<GLuint> gTexturesList;

/*
get current system time: std::chrono::system_clock::now()
get time since epoch: .time_since_epoch()
translate the underlying unit to milliseconds: duration_cast<milliseconds>(d)
translate std::chrono::milliseconds to integer (uint64_t to avoid overflow)
*/
uint64_t timeSinceEpochMillisec() 
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

/*
	Reads the shader file and returns the contents of it in a array of null-terminated sequence of characters
*/
std::string readShader(const char* fileName)
{
	// local variables
	std::string shaderCode;
	std::ifstream shaderFile;
	std::stringstream shaderStream;

	// ensure ifstream objects can throw exceptions
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(fileName);
		// read file's buffers contents into the stream
		shaderStream << shaderFile.rdbuf();

		// close file handler
		shaderFile.close();

		// convert stream to string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return shaderCode;
}


void draw(GLuint vertexCount, bool isIndexed)
{
	if (isIndexed) // if indexed drawing requested
	{
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
}

long getTimeInSecondsSinceOpenGLIsInitialized()
{
	return (timeSinceEpochMillisec() - openGLInitializationTime);
}