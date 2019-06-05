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
GLuint gLocLightAmbient;
GLuint gLocLightDiffuse;
GLuint gLocLightSpecular;
GLuint gLocLightPosition;
GLuint gLocViewerPosition;
GLuint gLocMaterialAmbient;
GLuint gLocMaterialDiffuse;
GLuint gLocMaterialSpecular;
GLuint gLocMaterialShininess;

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
	gLocLightAmbient = glGetUniformLocation(gShaderProgramEntity, "light.ambient");
	gLocLightDiffuse = glGetUniformLocation(gShaderProgramEntity, "light.diffuse");
	gLocLightSpecular = glGetUniformLocation(gShaderProgramEntity, "light.specular");
	gLocLightPosition = glGetUniformLocation(gShaderProgramEntity, "light.position");
	gLocViewerPosition = glGetUniformLocation(gShaderProgramEntity, "u_viewer_position");
	gLocMaterialAmbient = glGetUniformLocation(gShaderProgramEntity, "material.ambient");
	gLocMaterialDiffuse = glGetUniformLocation(gShaderProgramEntity, "material.diffuse");
	gLocMaterialSpecular = glGetUniformLocation(gShaderProgramEntity, "material.specular");
	gLocMaterialShininess = glGetUniformLocation(gShaderProgramEntity, "material.shininess");
}


void loadObjectColor(glm::vec3 objectColor)
{
	setVector3v(gLocObjectColor, objectColor);

}

void loadLightPosition(glm::vec3 lightPos)
{
	setVector3v(gLocLightPosition, lightPos);

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

void loadMaterialProperties(struct Material material)
{
	setVector3v(gLocMaterialAmbient, material.ambient);
	setVector3v(gLocMaterialDiffuse, material.diffuse);
	setVector3v(gLocMaterialSpecular, material.specular);
	setFloat(gLocMaterialShininess, material.shininess);
}

void loadLightProperties(struct Light light)
{
	setVector3v(gLocLightAmbient, light.ambient);
	setVector3v(gLocLightDiffuse, light.diffuse);
	setVector3v(gLocLightSpecular, light.specular);
	setVector3v(gLocLightPosition, light.lightPosition);
}