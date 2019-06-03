// headers
#include"MathUtils.h"

glm::mat4 creatTranslationMatrix(glm::mat4 translationMatrix, glm::vec3 translateBy)
{
	return glm::translate(translationMatrix, translateBy);
}