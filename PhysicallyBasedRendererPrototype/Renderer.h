#pragma once

// Internal includes
#include "Colour.h"
#include "Ray.h"
#include "Scene.h"

namespace TSFYP
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Render(const Scene& scene, const unsigned short imageWidth, const unsigned short imageHeight);

	private:
		Colour Li(const Ray& ray, const Scene& scene);
	};
}