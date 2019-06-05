#ifndef H_ENTITY_RENDERER
#define H_ENTITY_RENDERER

// headers
#include<iostream>
#include<vector>
#include"Models.h"
#include"GameUtils.h"
#include"ShaderProgram.h"
#include"EntityShaderProgram.h"
#include"Camera.h"
#include"Loader.h"
#include"OpenGLUtils.h"

void prepareEntityRenderer(glm::mat4 projectionMatrix);
void processEntity(struct Entity entity); // add entity to the entity list for rendering
void renderEntity(struct Light light); // render entities
void cleanUpEntities(); // clean up the shader program object
void increateEntityRotation(float rotX, float rotY, float rotZ);
#endif // !H_ENTITY_RENDERER
