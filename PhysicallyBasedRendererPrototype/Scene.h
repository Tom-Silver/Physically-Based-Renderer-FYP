#pragma once

// External includes
#include <vector>

// Internal includes
#include "SceneObject.h"

namespace TSFYP
{
	struct IntersectionResult;
	struct Ray;

	class Scene
	{
	public:
		Scene();

		bool Intersects(const Ray& ray, const IntersectionResult* result) const;

	private:
		std::vector<SceneObject> objects;
	};
}