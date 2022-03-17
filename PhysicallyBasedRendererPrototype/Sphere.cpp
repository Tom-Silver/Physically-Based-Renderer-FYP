#include "Sphere.h"

// External includes
#include <glad/glad.h>
#include <math.h>
#include <vector>

namespace TSFYP
{
    Sphere* CreateSphere(const float radius)
    {
        unsigned int vao = 0;
        glGenVertexArrays(1, &vao);

        unsigned int vbo = 0;
        glGenBuffers(1, &vbo);

        unsigned int ebo = 0;
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> vertexPositions;
        std::vector<glm::vec2> vertexTexCoords;
        std::vector<glm::vec3> vertexNormals;
        std::vector<unsigned int> indices;

        const unsigned int xSegments = 128;
        const unsigned int ySegments = 128;

        // Generate vertices
        {
            const float pi = 3.14159265359f;

            for (unsigned int x = 0; x <= xSegments; x++)
            {
                float xSegment = (float)x / (float)xSegments;

                for (unsigned int y = 0; y <= ySegments; y++)
                {
                    float ySegment = (float)y / (float)ySegments;
                    float xPos = cosf(xSegment * 2.0f * pi) * sinf(ySegment * pi);
                    float yPos = cosf(ySegment * pi);
                    float zPos = sinf(xSegment * 2.0f * pi) * sinf(ySegment * pi);

                    vertexPositions.emplace_back(glm::vec3(xPos, yPos, zPos));
                    vertexTexCoords.emplace_back(glm::vec2(xSegment, ySegment));
                    vertexNormals.emplace_back(glm::vec3(xPos, yPos, zPos));
                }
            }
        }

        // Generate indices
        unsigned int indexCount = 0;
        {
            bool oddRow = false;
            for (unsigned int y = 0; y < ySegments; y++)
            {
                if (oddRow)
                {
                    for (int x = xSegments; x >= 0; x--)
                    {
                        indices.emplace_back((y + 1) * (xSegments + 1) + x);
                        indices.emplace_back(y * (xSegments + 1) + x);
                    }
                }
                else
                {
                    for (unsigned int x = 0; x <= xSegments; x++)
                    {
                        indices.emplace_back(y * (xSegments + 1) + x);
                        indices.emplace_back((y + 1) * (xSegments + 1) + x);
                    }
                }

                oddRow = !oddRow;
            }

            indexCount = (float)indices.size();
        }

        // Store vertex data sequentially
        std::vector<float> data;
        for (unsigned int i = 0; i < vertexPositions.size(); i++)
        {
            data.emplace_back(vertexPositions[i].x);
            data.emplace_back(vertexPositions[i].y);
            data.emplace_back(vertexPositions[i].z);
            if (vertexNormals.size() > 0)
            {
                data.emplace_back(vertexNormals[i].x);
                data.emplace_back(vertexNormals[i].y);
                data.emplace_back(vertexNormals[i].z);
            }
            if (vertexTexCoords.size() > 0)
            {
                data.emplace_back(vertexTexCoords[i].x);
                data.emplace_back(vertexTexCoords[i].y);
            }
        }

        // Set buffer data
        {
            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            unsigned int stride = (3 + 2 + 3) * sizeof(float); // Pos + tex + normal
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        }

        return new Sphere(vao, indexCount, radius);
    }
}