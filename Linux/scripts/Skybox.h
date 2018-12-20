#ifndef SKYBOX_H
#define SKYBOX_H

#define HELPERGL_EXTERN_GL_FUNC_IMPLEMENTATION
#include "helper_gl.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Skybox
{
public:
	Skybox();
    ~Skybox();

	void display(float* view, float* projection);

	GLuint getID();

private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_texture;
	GLuint m_program;
	GLuint m_vertShader;
	GLuint m_fragShader;
};

#endif