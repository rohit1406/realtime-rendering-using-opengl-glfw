#include"SunShaderProgram.h"

using namespace std;

GLuint gShaderProgramSun;
//GLuint gLocColor;
GLuint gLocModelMatrixSun;
GLuint gLocViewMatrixSun;
GLuint gLocProjectionMatrixSun;


// gets all uniform locations
void getAllUniformLocationsSun()
{
	//gLocColor = glGetUniformLocation(gShaderProgram, "u_vertex_color");
	gLocModelMatrixSun = glGetUniformLocation(gShaderProgramSun, "u_model_matrix");
	gLocViewMatrixSun = glGetUniformLocation(gShaderProgramSun, "u_view_matrix");
	gLocProjectionMatrixSun = glGetUniformLocation(gShaderProgramSun, "u_projection_matrix");
}

// loads vertex color
/*void loadVertexColorSun(vector<GLfloat> color)
{
	setVector4v(gLocColorSun, color);
}*/

void loadModelMatrixSun(glm::mat4 transformationMatrix)
{
	setMat4(gLocModelMatrixSun, transformationMatrix);
}

void loadViewMatrixSun(glm::mat4 transformationMatrix)
{
	setMat4(gLocViewMatrixSun, transformationMatrix);
}

void loadProjectionMatrixSun(glm::mat4 transformationMatrix)
{
	setMat4(gLocProjectionMatrixSun, transformationMatrix);
}