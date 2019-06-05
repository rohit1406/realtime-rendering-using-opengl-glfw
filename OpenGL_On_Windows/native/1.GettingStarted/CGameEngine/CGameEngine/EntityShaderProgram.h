#ifndef H_SHADERPROGRAM_ENTITY
#define H_SHADERPROGRAM_ENTITY

#include<iostream>
#include<stdio.h>
#include<vector>
#include<Windows.h>

#define GLEW_STATIC
#include<GL/glew.h>
#include<gl/GL.h> // for OpenGL

#include"ShaderUtils.h"
#include"GameUtils.h"
#include"Models.h"
#include"MathUtils.h"
#include"Logger.h"

void getAllUniformLocations(); // get uniform locations

// uniforms
extern GLuint gShaderProgramEntity;
extern GLuint gLocColor;
extern GLuint gLocPositionOffset;
extern GLuint gLocBricksSampler;
extern GLuint gLocFaceSampler;
extern GLuint gLocMixParam;
extern GLuint gLocModelMatrix;
extern GLuint gLocViewMatrix;
extern GLuint gLocProjectionMatrix;
extern GLuint gLocObjectColor;
extern GLuint gLocLightColor;
extern GLuint gLocLightPosition;
extern GLuint gLocViewerPosition;

void loadPositionOffset(float offset);
void bindTextureUnits(struct TexturedModel modelTexture);
void loadBricksTextureSampler(GLuint samplerID);
void loadFaceTextureSampler(GLuint samplerID);
void loadMixParam(GLfloat value);
void loadModelMatrix(glm::mat4 transformationMatrix);
void loadViewMatrix(glm::mat4 transformationMatrix);
void loadProjectionMatrix(glm::mat4 transformationMatrix);
void loadObjectColor(glm::vec3 objectColor);
void loadLightColor(glm::vec3 lightColor);
void loadLightPosition(glm::vec3 lightPos);
void loadViewerPosition(glm::vec3 viewerPos);
#endif // !H_SHADERPROGRAM_ENTITY
