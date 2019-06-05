// headers
#include"SunRenderer.h"

// global variables
extern GLuint gShaderProgramSun;
std::vector<struct Light> lights;
extern struct Camera camera; // for view matrix

void setUpLight(struct Light* light, struct TexturedModel *texturedModel)
{
	light->entity.texturedModel = *texturedModel;
	light->entity.translate = glm::vec3(1.2f, 1.0f, 2.0f);
	light->entity.rotateX = 0;
	light->entity.rotateY = 0;
	light->entity.rotateZ = 0;
	light->entity.scale = 0.2f;
	light->lightColor = glm::vec3(1, 1, 1);
	light->lightPosition = light->entity.translate;
	light->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	light->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void prepareSunRenderer(glm::mat4 projectionMatrix)
{
	// local variables
	const std::string vertexFile = SHADER_RESOURCE_FILE_LOC + std::string("sun.vs");
	const std::string fragmentFile = SHADER_RESOURCE_FILE_LOC + std::string("sun.fs");

	// code
	// create shader program object
	gShaderProgramSun = buildShaderProgramObject(vertexFile.c_str(), fragmentFile.c_str());
	logStaticData("Sun Shader program object created.");

	// Load uniforms
	getAllUniformLocationsSun(); // get the uniform locations
	logStaticData("Loaded Sun uniform locations.");

	startProgram(gShaderProgramSun);
	loadProjectionMatrixSun(projectionMatrix);
	stopProgram();
}

// add entity to the entity list for rendering
void processSun(struct Light light)
{
	lights.push_back(light);
}

// render sun
void renderSun()
{
	// start shader program
	startProgram(gShaderProgramSun);
	for (int i = 0; i < lights.size(); i++)
	{
		// bind VAO
		glBindVertexArray(lights[i].entity.texturedModel.rawModel.vaoID);
		glm::mat4 transformationMatrix = glm::mat4(1.0);

		// load uniforms
		loadModelMatrixSun(getTransformationMatrix(transformationMatrix, lights[i].entity));
		loadViewMatrixSun(createViewMatrix(camera));

		// draw entity
		draw(lights[i].entity.texturedModel.rawModel.vertexCount, false);

		// unbind VAO
		glBindVertexArray(0);
	}

	// stop shader program
	stopProgram();
}

// clean up the shader program object
void cleanUpSun()
{
	lights.clear();
}

