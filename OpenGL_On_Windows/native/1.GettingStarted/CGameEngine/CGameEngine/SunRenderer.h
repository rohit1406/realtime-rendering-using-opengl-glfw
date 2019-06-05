#ifndef H_LIGHT
#define H_LIGHT

// headers
#include"Models.h"
#include"GameUtils.h"
#include"ShaderProgram.h"
#include"OpenGLUtils.h"
#include"SunShaderProgram.h"
#include"Camera.h"

void setUpLight(struct Light*light, struct TexturedModel *texturedModel);
void prepareSunRenderer(glm::mat4 projectionMatrix);
void processSun(struct Light sun); // add entity to the entity list for rendering
void renderSun(); // render entities
void cleanUpSun(); // clean up the shader program object
#endif // !H_LIGHT
