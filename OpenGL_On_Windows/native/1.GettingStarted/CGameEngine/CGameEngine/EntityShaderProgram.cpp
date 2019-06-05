#include"ShaderProgram.h"

using namespace std;

GLuint gShaderProgramEntity;
GLuint gLocColor;
GLuint gLocPositionOffset;
GLuint gLocBricksSampler;
GLuint gLocFaceSampler;
GLuint gLocMixParam;
GLuint gLocModelMatrix;
GLuint gLocViewMatrix;
GLuint gLocProjectionMatrix;
GLuint gLocObjectColor;
GLuint gLocLightColor;
GLuint gLocLightPosition;
GLuint gLocViewerPosition;


// gets all uniform locations
void getAllUniformLocations()
{
	gLocColor = glGetUniformLocation(gShaderProgramEntity, "u_vertex_color");
	gLocPositionOffset = glGetUniformLocation(gShaderProgramEntity, "u_position_offset");
	gLocBricksSampler = glGetUniformLocation(gShaderProgramEntity, "u_bricks_sampler");
	gLocFaceSampler = glGetUniformLocation(gShaderProgramEntity, "u_face_sampler");
	gLocMixParam = glGetUniformLocation(gShaderProgramEntity, "u_mix_texture_param");
	gLocModelMatrix = glGetUniformLocation(gShaderProgramEntity, "u_model_matrix");
	gLocViewMatrix = glGetUniformLocation(gShaderProgramEntity, "u_view_matrix");
	gLocProjectionMatrix = glGetUniformLocation(gShaderProgramEntity, "u_projection_matrix");
	gLocObjectColor = glGetUniformLocation(gShaderProgramEntity, "u_object_color");
	gLocLightColor = glGetUniformLocation(gShaderProgramEntity, "u_light_color");
	gLocLightPosition = glGetUniformLocation(gShaderProgramEntity, "u_light_position");
	gLocViewerPosition = glGetUniformLocation(gShaderProgramEntity, "u_viewer_position");
}


void loadObjectColor(glm::vec3 objectColor)
{
	setVector3v(gLocObjectColor, objectColor);

}

void loadLightPosition(glm::vec3 lightPos)
{
	setVector3v(gLocLightPosition, lightPos);

}

void loadLightColor(glm::vec3 lightColor)
{
	setVector3v(gLocLightColor, lightColor);
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

void loadViewerPosition(glm::vec3 viewerPos)
{
	setVector3v(gLocViewerPosition, viewerPos);
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