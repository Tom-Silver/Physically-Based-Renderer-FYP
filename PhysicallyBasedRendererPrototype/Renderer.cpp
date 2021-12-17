#include "Renderer.h"

// External includes
#include <algorithm>

// Internal includes
#include "GeometryOperations.h"
#include "Scene.h"

namespace TSFYP
{
	void WriteToFile(const char* fileName, const unsigned short imageWidth, const unsigned short imageHeight, Colour* image);

	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::Render(const Scene& scene, const unsigned short imageWidth, const unsigned short imageHeight)
	{
		Colour* image = new Colour[imageWidth * imageHeight];
		Colour* pixel = image;

		for (unsigned short x = 0; x < imageWidth; x++)
		{
			for (unsigned short y = 0; y < imageHeight; y++, pixel++)
			{
				Ray ray;
				ray.dir = Vector3(x, y, -1.0f);
				ray.origin = Point3(0.0f, 0.0f, 0.0f);

				*pixel = Li(ray, scene);
			}
		}

		// Output image to file
		WriteToFile("output.ppm", imageWidth, imageHeight, image);
	}

	Colour Renderer::Li(const Ray& ray, const Scene& scene)
	{
		IntersectionResult* result = nullptr;
		if (!scene.Intersects(ray, result) || !result)
		{
			return Colour::Black;
		}

		return result->intersectedObject->colour;
	}

	void WriteToFile(const char* fileName, const unsigned short imageWidth, const unsigned short imageHeight, Colour* image)
	{
		// Create a buffer to write image into.This will allow the entire image to be written to file at once, which is a lot more efficient than doing it pixel by pixel
			unsigned char* buffer = new unsigned char[(long long)imageWidth * (long long)imageHeight * 3];
		unsigned j = 0;
		for (unsigned int i = 0; i < imageWidth * imageHeight; ++i)
		{
			buffer[j] = (unsigned char)(std::min(1.0f, image[i].r) * 255.0f);
			buffer[j + 1] = (unsigned char)(std::min(1.0f, image[i].g) * 255.0f);
			buffer[j + 2] = (unsigned char)(std::min(1.0f, image[i].b) * 255.0f);

			j += 3;
		}

		FILE* file;
		fopen_s(&file, fileName, "wb");
		if (!file)
			return;

		fprintf(file, "P6\n%d %d\n255\n", imageWidth, imageHeight);
		fwrite(buffer, sizeof(buffer[0]), (long long)imageWidth * (long long)imageHeight * 3, file); // Write buffer containing image data to file

		fclose(file);
	}
}