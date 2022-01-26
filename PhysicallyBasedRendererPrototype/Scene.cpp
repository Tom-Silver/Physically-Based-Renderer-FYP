#include "Scene.h"

// External includes
#include <limits>

// Internal includes
#include "GeometryOperations.h"
#include "Sphere.h"

namespace TSFYP
{
	Scene::Scene(const std::vector<SceneObject>& objects, const std::vector<Light>& lights)
		: mObjects(objects)
		, mLights(lights)
		, mCamera(90.0f)
	{}

	bool Scene::Intersects(const Ray& ray, IntersectionResult* result) const
	{
		const SceneObject* closestObject = nullptr;
		float closest = std::numeric_limits<float>::max();
		for (const SceneObject& object : mObjects)
		{
			IntersectionResult newResult;
			if (object.shape->Intersects(ray, *object.transform, &newResult))
			{
				if (newResult.t < closest)
				{
					*result = newResult;

					closest = result->t;
					closestObject = &object;
				}
			}
		}

		if (closestObject)
		{
			result->intersectedObject = closestObject;
			return true;
		}

		return false;
	}
}