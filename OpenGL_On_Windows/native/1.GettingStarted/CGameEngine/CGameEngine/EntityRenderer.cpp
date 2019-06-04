// headers
#include"EntityRenderer.h"

using namespace std;

// global variables
std::vector<Entity> entities;
GLuint giEntityShaderProgram;
extern float gMixParam;
extern struct Camera camera;

// prepare the entities for rendering
void prepareEntityRenderer(glm::mat4 projectionMatrix)
{
	// local variables
	const string vertexFile = SHADER_RESOURCE_FILE_LOC + string("vertexShader.vs");
	const string fragmentFile = SHADER_RESOURCE_FILE_LOC + string("fragmentShader.fs");

	// code
	// create shader program object
	giEntityShaderProgram = buildShaderProgramObject(vertexFile.c_str(), fragmentFile.c_str());
	logStaticData("Shader program object created.");

	// Load uniforms
	getAllUniformLocations(); // get the uniform locations
	logStaticData("Loaded uniform locations.");

	startProgram(giEntityShaderProgram);
	loadProjectionMatrix(projectionMatrix);
	loadBricksTextureSampler(0);
	loadFaceTextureSampler(1);
	stopProgram();
}

// add entity to the entity list for rendering
void processEntity(struct Entity entity)
{
	entities.push_back(entity);
}

// render entities
void renderEntity()
{
	startProgram(giEntityShaderProgram);
	for (int i = 0; i < entities.size(); i++)
	{
		glBindVertexArray(entities[i].texturedModel.rawModel.vaoID);
		glm::mat4 transformationMatrix = glm::mat4(1.0);
		loadModelMatrix(getTransformationMatrix(transformationMatrix, entities[i]));
		loadViewMatrix(createViewMatrix(camera));
		loadMixParam(gMixParam);
		bindTextureUnits(entities[i].texturedModel);
		draw(entities[i].texturedModel.rawModel.vertexCount, false);
		glBindVertexArray(0);
	}
	stopProgram();
}

// clean up activities
void cleanUpEntities()
{
	for (int i = 0; i < entities.size(); i++)
	{
		// delete VAOS
		if (entities[i].texturedModel.rawModel.vaoID)
		{
			glDeleteVertexArrays(1, &entities[i].texturedModel.rawModel.vaoID);
			entities[i].texturedModel.rawModel.vaoID = 0;
		}
	}
	entities.clear();
}