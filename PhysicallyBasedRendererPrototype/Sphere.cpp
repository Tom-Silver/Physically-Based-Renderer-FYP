#include "Sphere.h"

// External includes
#include <glad/glad.h>
#include <math.h>
#include <vector>

namespace TSFYP
{
    Sphere* CreateSphere(const float radius, const unsigned int segmentCircumference)
    {
        // Create buffer objects
        unsigned int vao = 0;
        glGenVertexArrays(1, &vao);

        unsigned int vbo = 0;
        glGenBuffers(1, &vbo);

        unsigned int ebo = 0;
        glGenBuffers(1, &ebo);

        // Generate vertices
        std::vector<glm::vec3> vertexPositions;
        std::vector<glm::vec2> vertexTexCoords;
        std::vector<glm::vec3> vertexNormals;
        {
            const float pi = 3.14159265359f;

            for (unsigned int x = 0; x <= segmentCircumference; x++)
            {
                float xSegment = (float)x / (float)segmentCircumference;

                for (unsigned int y = 0; y <= segmentCircumference; y++)
                {
                    float ySegment = (float)y / (float)segmentCircumference;
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
        std::vector<unsigned int> indices;
        {
            bool oddRow = false;
            for (unsigned int y = 0; y < segmentCircumference; y++)
            {
                if (oddRow)
                {
                    for (int x = segmentCircumference; x >= 0; x--)
                    {
                        indices.emplace_back((y + 1) * (segmentCircumference + 1) + x);
                        indices.emplace_back(y * (segmentCircumference + 1) + x);
                    }
                }
                else
                {
                    for (unsigned int x = 0; x <= segmentCircumference; x++)
                    {
                        indices.emplace_back(y * (segmentCircumference + 1) + x);
                        indices.emplace_back((y + 1) * (segmentCircumference + 1) + x);
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

        {
            glBindVertexArray(vao);

            // Set buffer data
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            // Link vertex attribs
            unsigned int stride = (3 + 3 + 2) * sizeof(float); // Pos + normal + tex

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        return new Sphere(vao, indexCount, radius);
    }
}