#include"ShaderProgram.h"

using namespace std;

vector<GLuint> giShaderList;
vector<GLuint> giShaderProgramObjectList;

// builds the vertex shader
GLuint buildVertexShader()
{
	// local variables
	GLuint vertexShader;

	// shader source code
	const GLchar* vertexShaderSourceCode = 
		"#version 330 core"\
		"\n"\
		"layout (location = 0) in vec3 aPosition;"\
		"void main()"\
		"{"\
		"gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);"\
		"}";

	// code
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // creates vertex shader object
	// attache source code to the above shader
	glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
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
		cout << "ERROR::COMPILATION_FAILURE::Vertex Shader Logs: " << infoLog << endl;
	}
	giShaderList.push_back(vertexShader); // for cleanUp
	return vertexShader;
}

// builds fragment shader
GLuint buildFragmentShader()
{
	// local variables
	GLuint fragmentShader;

	// shader source code
	const GLchar* fragmentShaderSourceCode =
		"#version 330 core"\
		"\n"\
		"out vec4 FragColor;"\
		"void main()"\
		"{"\
		"FragColor = vec4(1.0, 0.5, 0.2, 1.0);"\
		"}";

	// code
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // creates fragment shader object
	// attache source code to the above shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
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
		cout << "ERROR::COMPILATION_FAILURE::Fragment Shader Logs: " << infoLog << endl;
	}
	giShaderList.push_back(fragmentShader); // for cleanUp
	return fragmentShader;
}

// builds the shader program object
GLuint buildShaderProgramObject()
{
	// local variables
	GLuint shaderProgramObject;

	// code
	// get the vertex shader and fragment shader objects
	GLuint vertexShader = buildVertexShader();
	GLuint fragmentShader = buildFragmentShader();

	// create shader program
	shaderProgramObject = glCreateProgram();

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
		cout << "ERROR::LINKAGE_FAILURE::Linkage error Logs: " << infoLog << endl;
	}
	giShaderProgramObjectList.push_back(shaderProgramObject);
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
