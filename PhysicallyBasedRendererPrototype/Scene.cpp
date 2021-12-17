#include "Scene.h"

// External includes
#include <limits>

// Internal includes
#include "GeometryOperations.h"
#include "Sphere.h"

namespace TSFYP
{
	Scene::Scene()
	{
		SceneObject* object = new SceneObject();
		object->shape = new Sphere(5.0f);
		object->transform = new Transform();
		this->objects.push_back(*object);
	}

	bool Scene::Intersects(const Ray& ray, const IntersectionResult* result) const
	{
		const SceneObject* closestObject = nullptr;
		float closest = std::numeric_limits<float>::max();
		for (const SceneObject& object : this->objects)
		{
			IntersectionResult* newResult = nullptr;
			if (object.shape->Intersects(ray, newResult) && newResult)
			{
				if (newResult->t < closest)
				{
					delete result;
					result = newResult;

					closest = result->t;
					closestObject = &object;
				}
			}
		}

		if (closestObject)
			return true;

		return false;
	}
}