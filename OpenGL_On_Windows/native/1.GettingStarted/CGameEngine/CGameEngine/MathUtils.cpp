// headers
#include"MathUtils.h"

glm::mat4 creatTranslationMatrix(glm::mat4 translationMatrix, glm::vec3 translateBy)
{
	return glm::translate(translationMatrix, translateBy);
}


// creates orthographic projection matrix
glm::mat4 createOrthographicProjectionMatrix()
{
	return glm::ortho(
		0.0f, // left
		800.0f, // right
		0.0f, // bottom
		600.0f, // top
		0.1f, // neaf
		100.0f // far
	);
}

glm::mat4 createPerspectiveProjectionMatrix(int width, int height)
{
	glm::mat4 proj = glm::perspective(
		glm::radians(45.0f), // fovy
		(float) width/ (float) height, // aspect ratio
		0.1f, // near
		100.0f // far
	);
	return proj;
}

glm::mat4 createViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	return view;
}