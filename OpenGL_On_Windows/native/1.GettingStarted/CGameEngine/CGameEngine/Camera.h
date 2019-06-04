#ifndef H_CAMERA
#define H_CAMERA

// headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Logger.h"
// camera
struct Camera
{
	glm::vec3 position;
	glm::vec3 target; // looking at
	glm::vec3 up;
	glm::vec3 cameraFront;
};

extern struct Camera camera;
extern glm::vec3 gCameraPosition;
void initializeCamera();
void updateCameraPosition(glm::vec3 position);
#endif // !H_CAMERA
