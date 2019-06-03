#ifndef H_MATH_UTILS
#define H_MATH_UTILS

/*
https://learnopengl.com/Getting-started/Transformations for transformation, scaling and rotation of the matrices
*/
// headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 creatTranslationMatrix(glm::mat4 translationMatrix, glm::vec3 translateBy);
#endif // !H_MATH_UTILS
