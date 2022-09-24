#include "Quad.h"

// External includes
#include <glad/glad.h>

namespace TSFYP
{
	Quad::Quad(const unsigned int vao, const unsigned int indexCount)
		: Mesh(vao, indexCount)
	{}

	Quad::~Quad()
	{}

	Quad* CreateQuad()
	{
        // Create buffer objects
        unsigned int vao = 0;
        glGenVertexArrays(1, &vao);

        unsigned int vbo = 0;
        glGenBuffers(1, &vbo);

        // Create vertices
        float vertices[] = {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f
        };

        // Set buffer data
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Link vertex attribs
        {
            glBindVertexArray(vao);

            unsigned int stride = (3 + 2) * sizeof(float); // Pos + tex

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        return new Quad(vao, 0);
	}
}