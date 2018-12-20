#include "Skybox.h"

#include "Bitmap.h"
#include "Helper.h"


Skybox::Skybox()
{
	// Program
	char* data_vert = ReadFile("shaders/Skybox.vs");
	char* data_frag = ReadFile("shaders/Skybox.fs");

	m_vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertShader, 1, &data_vert, NULL);
	glCompileShader(m_vertShader);

	m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragShader, 1, &data_frag, NULL);
	glCompileShader(m_fragShader);
	
	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertShader);
	glAttachShader(m_program, m_fragShader);
	glLinkProgram(m_program);
	
	// Object
	float skyboxVertices[] = 
	{
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Texture
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	
	Bitmap bmp = Bitmap::bitmapFromFile("textures/cubemap1.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGB, bmp.width(), bmp.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.pixelBuffer());

	bmp = Bitmap::bitmapFromFile("textures/cubemap2.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, bmp.width(), bmp.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.pixelBuffer());

	bmp = Bitmap::bitmapFromFile("textures/cubemap3.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, bmp.width(), bmp.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.pixelBuffer());
	
	bmp = Bitmap::bitmapFromFile("textures/cubemap4.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, bmp.width(), bmp.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.pixelBuffer());

	bmp = Bitmap::bitmapFromFile("textures/cubemap5.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, bmp.width(), bmp.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.pixelBuffer());

	bmp = Bitmap::bitmapFromFile("textures/cubemap6.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB, bmp.width(), bmp.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.pixelBuffer());

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
Skybox::~Skybox()
{
	glDetachShader(m_program, m_vertShader);
	glDetachShader(m_program, m_fragShader);
	glDeleteShader(m_vertShader);
	glDeleteShader(m_fragShader);
	glDeleteProgram(m_program);
}

void Skybox::display(float* view, float* projection)
{
	glDepthFunc(GL_LEQUAL);
	glUseProgram(m_program);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, projection);

	glBindVertexArray(m_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);

	glUseProgram(0);
	glDepthFunc(GL_LESS);
}

GLuint Skybox::getID()
{
	return m_texture;
}