// header
#include"Camera.h"

// global variables
struct Camera camera;

glm::vec3 gCameraPosition(camera.position);

void initializeCamera()
{
	camera.position = glm::vec3(0.0, 0.0, 3.0), // position
		camera.cameraFront = glm::vec3(0.0, 0.0, -1.0); // in front of camera
	camera.up = glm::vec3(0.0, 1.0, 0.0); // up axis

	gCameraPosition = camera.position;
	logStaticData("Camera initialized");
}

void updateCameraPosition(glm::vec3 position)
{
	camera.position = position;
}