#include <math.h>
#include <assert.h>
#include <stdio.h>
// OpenGL Graphics includes
#define HELPERGL_EXTERN_GL_FUNC_IMPLEMENTATION
#include "helper_gl.h"
#include "render_particles.h"

#include "Bitmap.h"
#include "Helper.h"

#ifndef M_PI
#define M_PI    3.1415926535897932384626433832795
#endif

ParticleRenderer::ParticleRenderer(): m_pos(0), m_numParticles(0), m_pointSize(1.0f), m_particleRadius(0.125f * 0.5f), m_program_normal(0), m_program_reflect(0), m_program_refract(0), m_vbo(0), m_colorVBO(0){
    _initGL();
	_initTexture();
}
ParticleRenderer::~ParticleRenderer(){
    m_pos = 0;
}
void ParticleRenderer::setPositions(float *pos, int numParticles){
    m_pos = pos;
    m_numParticles = numParticles;
}
void ParticleRenderer::setVertexBuffer(unsigned int vbo, int numParticles){
    m_vbo = vbo;
    m_numParticles = numParticles;
}


void ParticleRenderer::_drawPoints(){
    if (!m_vbo)
    {
        glBegin(GL_POINTS);
        {
            int k = 0;

            for (int i = 0; i < m_numParticles; ++i)
            {
                glVertex3fv(&m_pos[k]);
                k += 4;
            }
        }
        glEnd();
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexPointer(4, GL_FLOAT, 0, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        if (m_colorVBO){
            glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
            glColorPointer(4, GL_FLOAT, 0, 0);
            glEnableClientState(GL_COLOR_ARRAY);
        }
        glDrawArrays(GL_POINTS, 0, m_numParticles);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
}
void ParticleRenderer::display(DisplayMode mode, float* cameraPos){
	if(mode == PARTICLE_POINTS)
	{
		glColor3f(1, 1, 1);
		glPointSize(m_pointSize);
		_drawPoints();
	}
	else
	{
		glEnable(GL_POINT_SPRITE_ARB);
		glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);

		GLuint program;
		switch(mode)
		{
			default:
			case PARTICLE_SPHERES:
				program = m_program_normal;
				break;
			case PARTICLE_REFLECT:
				program = m_program_reflect;
				break;
			case PARTICLE_REFRACT:
				program = m_program_refract;
				break;
			case PARTICLE_TEXTURE:
				program = m_program_texture;
				break;
		}

		glUseProgram(program);

		glUniform1f(glGetUniformLocation(program, "pointScale"), m_window_h / tanf(m_fov*0.5f*(float)M_PI/180.0f));
		glUniform1f(glGetUniformLocation(program, "pointRadius"), m_particleRadius);

		glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPos[0], cameraPos[1], cameraPos[2]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTex);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_particleTex);

		glColor3f(1, 1, 1);
		_drawPoints();

		glUseProgram(0);
		glDisable(GL_POINT_SPRITE_ARB);
	}
}

GLuint ParticleRenderer::_compileProgram(const char *vsource, const char *fsource)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vsource, 0);
    glShaderSource(fragmentShader, 1, &fsource, 0);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    // check if program linked
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        char temp[256];
        glGetProgramInfoLog(program, 256, 0, temp);
        printf("Failed to link program:\n%s\n", temp);
        glDeleteProgram(program);
        program = 0;
    }

    return program;
}

void ParticleRenderer::_initGL()
{
    char* data_vertex;
	char* data_fragment;

	data_vertex = ReadFile("shaders/Sphere_normal.vs");
	data_fragment = ReadFile("shaders/Sphere_normal.fs");
	m_program_normal = _compileProgram(data_vertex, data_fragment);

	data_vertex = ReadFile("shaders/Sphere_reflect.vs");
	data_fragment = ReadFile("shaders/Sphere_reflect.fs");
	m_program_reflect = _compileProgram(data_vertex, data_fragment);

	data_vertex = ReadFile("shaders/Sphere_refract.vs");
	data_fragment = ReadFile("shaders/Sphere_refract.fs");
	m_program_refract = _compileProgram(data_vertex, data_fragment);

	data_vertex = ReadFile("shaders/Sphere_texture.vs");
	data_fragment = ReadFile("shaders/Sphere_texture.fs");
	m_program_texture = _compileProgram(data_vertex, data_fragment);
}
void ParticleRenderer::_initTexture()
{
	// Particle
	glGenTextures(1, &m_particleTex);

	glBindTexture(GL_TEXTURE_2D, m_particleTex);

	Bitmap bmp = Bitmap::bitmapFromFile("textures/particle.jpg");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.width(), bmp.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.pixelBuffer());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture(GL_TEXTURE_2D, 0);
}