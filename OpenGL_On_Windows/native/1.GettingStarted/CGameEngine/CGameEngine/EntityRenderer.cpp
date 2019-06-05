// headers
#include"EntityRenderer.h"

using namespace std;

// global variables
std::vector<Entity> entities;
extern GLuint gShaderProgramEntity;
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
	gShaderProgramEntity = buildShaderProgramObject(vertexFile.c_str(), fragmentFile.c_str());
	logStaticData("Entity Shader program object created.");

	// Load uniforms
	getAllUniformLocations(); // get the uniform locations
	logStaticData("Loaded entity uniform locations.");

	startProgram(gShaderProgramEntity);
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
void renderEntity(struct Light light)
{
	// start shader program
	startProgram(gShaderProgramEntity);
	for (int i = 0; i < entities.size(); i++)
	{
		// bind VAO
		glBindVertexArray(entities[i].texturedModel.rawModel.vaoID);
		glm::mat4 transformationMatrix = glm::mat4(1.0);

		// load uniforms
		loadModelMatrix(getTransformationMatrix(transformationMatrix, entities[i]));
		loadViewMatrix(createViewMatrix(camera));
		loadMixParam(gMixParam);

		loadObjectColor(glm::vec3(1.0, 0.5f, 0.31f));
		loadLightColor(light.lightColor);
		loadLightPosition(light.lightPosition);
		loadViewerPosition(camera.position);
		// bind texture units
		bindTextureUnits(entities[i].texturedModel);

		// draw entity
		draw(entities[i].texturedModel.rawModel.vertexCount, false);

		// unbind VAO
		glBindVertexArray(0);
	}

	// stop shader program
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

void increateEntityRotation(float rotX, float rotY, float rotZ)
{
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i].rotateX = rotX;
		entities[i].rotateY = rotY;
		entities[i].rotateZ = rotZ;
	}
}