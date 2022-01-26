#pragma once

// Internal includes
#include "Colour.h"
#include "Ray.h"
#include "Scene.h"

namespace TSFYP
{
	// Handles the rendering of the final image
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		// Generates an image of the given scene
		void Render(const Scene& scene, const unsigned short imageWidth, const unsigned short imageHeight);

		void SetClearColour(const Colour& colour) { mClearColour = colour; }

	private:
		// Determines the colour of a pixel by tracing the given ray through the given scene
		Colour Li(const Ray& ray, const Scene& scene);

		Colour mClearColour;
	};
}