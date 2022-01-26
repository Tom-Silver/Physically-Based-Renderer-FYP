#pragma once

// External includes
#include <vector>

// Internal includes
#include "Camera.h"
#include "Light.h"
#include "SceneObject.h"

namespace TSFYP
{
	struct IntersectionResult;
	struct Ray;

	// Stores the 3D scene to be rendered, including the world objects, lights, and camera that will take the picture
	class Scene
	{
	public:
		Scene(const std::vector<SceneObject>& objects, const std::vector<Light>& lights);

		bool Intersects(const Ray& ray, IntersectionResult* result) const;

		std::vector<Light> mLights; // public to allow access to loop through lights during rendering
		const Camera camera() const { return mCamera; }

	private:
		Camera mCamera;
		std::vector<SceneObject> mObjects;
	};
}