// headers
#include"OpenGLStateChanger.h"

// enables wireframe mode
// wireframe mode shows that the model indeed consists of a triangles.
// It configures how OpenGL draws it's primitives
void enableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// disables wireframe mode
void disableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}