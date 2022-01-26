#include "Renderer.h"

// External includes
#include <algorithm>
#include <fstream>

// Internal includes
#include "GeometryOperations.h"
#include "Scene.h"
#include "Transform.h"

// Preprocessor definitions
#define PI 3.141592653589793f

namespace TSFYP
{
	void WriteToFile(const char* fileName, const unsigned short imageWidth, const unsigned short imageHeight, Colour* image);

	Renderer::Renderer()
		: mClearColour(Colour::Black)
	{}

	Renderer::~Renderer()
	{}

	void Renderer::Render(const Scene& scene, const unsigned short imageWidth, const unsigned short imageHeight)
	{
		long long imageSize = (long long)imageWidth * (long long)imageHeight;
		Colour* image = new Colour[imageSize];
		Colour* pixel = image;

		// Precalculate inverse width and height to save expensive division operations every loop
		float invWidth = 1.0f / float(imageWidth);
		float invHeight = 1.0f / float(imageHeight);

		// Also precalculate angle and aspectratio to avoid recalculation
		float fov = scene.camera().fov();
		float angle = tanf(PI * 0.5f * fov / 180.0f);

		float aspectratio = (float)imageWidth / float(imageHeight);

		for (unsigned short y = 0; y < imageHeight; y++)
		{
			for (unsigned short x = 0; x < imageWidth; x++, pixel++)
			{
				float xDir = (2.0f * ((x + 0.5f) * invWidth) - 1.0f) * angle * aspectratio;
				float yDir = (1.0f - 2.0f * ((y + 0.5f) * invHeight)) * angle;

				Ray ray;
				ray.dir = Normalise(Vector3(xDir, yDir, -1.0f));
				ray.origin = Point3(0.0f, 0.0f, 0.0f);

				*pixel = Li(ray, scene);
			}
		}

		// Output image to file
		WriteToFile("output.ppm", imageWidth, imageHeight, image);
	}

	Colour Renderer::Li(const Ray& ray, const Scene& scene)
	{
		IntersectionResult result;
		if (!scene.Intersects(ray, &result))
		{
			return mClearColour;
		}

		Vector3 surfaceColour = Vector3(result.intersectedObject->colour);

		Colour reflectedColour = mClearColour;
		for (const Light& light : scene.mLights)
		{
			Vector3 surfaceToLight = Vector3(light.pos()) - Vector3(result.intersectionPoint);
			Normalise(surfaceToLight);

			Colour litSurfaceColour = Colour(surfaceColour * std::max(0.0f, Dot(result.intersectionNormal, surfaceToLight)));
			Colour emittedColour = Colour(Vector3(light.emittedColour()) * std::max(0.0f, Dot(result.intersectionNormal, surfaceToLight)));
			reflectedColour += result.intersectedObject->colour + emittedColour; // isn't working as it should. why?
		}

		return reflectedColour;
	}

	void WriteToFile(const char* fileName, const unsigned short imageWidth, const unsigned short imageHeight, Colour* image)
	{
		// Create a buffer to write image into. This will allow the entire image to be written to file at once, which is a lot more efficient than doing it pixel by pixel
		long long imageSize = (long long)imageWidth * (long long)imageHeight;
		unsigned char* buffer = new unsigned char[imageSize * 3];
		
		unsigned int j = 0;
		for (unsigned int i = 0; i < imageSize; i++, j += 3)
		{
			buffer[j] = (unsigned char)(std::min(1.0f, image[i].r) * 255.0f);
			buffer[j + 1] = (unsigned char)(std::min(1.0f, image[i].g) * 255.0f);
			buffer[j + 2] = (unsigned char)(std::min(1.0f, image[i].b) * 255.0f);
		}

		FILE* file;
		fopen_s(&file, fileName, "wb");
		if (!file)
			return;

		fprintf(file, "P6\n%d %d\n255\n", imageWidth, imageHeight);
		fwrite(buffer, sizeof(buffer[0]), imageSize * 3, file); // Write buffer containing image data to file

		fclose(file);
	}
}