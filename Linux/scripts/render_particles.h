/*
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

#ifndef __RENDER_PARTICLES__
#define __RENDER_PARTICLES__

class ParticleRenderer
{
    public:
        ParticleRenderer();
        ~ParticleRenderer();

        void setPositions(float *pos, int numParticles);
        void setVertexBuffer(unsigned int vbo, int numParticles);
        void setColorBuffer(unsigned int vbo)
        {
            m_colorVBO = vbo;
        }

        enum DisplayMode
        {
            PARTICLE_POINTS,
            PARTICLE_SPHERES,
			PARTICLE_REFLECT,
			PARTICLE_REFRACT,
			PARTICLE_TEXTURE,
            PARTICLE_NUM_MODES
        };

        void display(DisplayMode mode = PARTICLE_POINTS, float* cameraPos = 0);
        void displayGrid();

        void setPointSize(float size)
        {
            m_pointSize = size;
        }
        void setParticleRadius(float r)
        {
            m_particleRadius = r;
        }
        void setFOV(float fov)
        {
            m_fov = fov;
        }
        void setWindowSize(int w, int h)
        {
            m_window_w = w;
            m_window_h = h;
        }

		void setSkyBox(GLuint cubeMapTex)
		{
			m_cubeMapTex = cubeMapTex;
		}

    protected: // methods
        void _initGL();
		void _initTexture();
        void _drawPoints();
        GLuint _compileProgram(const char *vsource, const char *fsource);

    protected: // data
        float *m_pos;
        int m_numParticles;

        float m_pointSize;
        float m_particleRadius;
        float m_fov;
        int m_window_w, m_window_h;

		GLuint m_program_normal;
        GLuint m_program_reflect;
		GLuint m_program_refract;
		GLuint m_program_texture;

        GLuint m_vbo;
        GLuint m_colorVBO;

		GLuint m_cubeMapTex;
		GLuint m_particleTex;
};

#endif //__ RENDER_PARTICLES__
