#pragma once

#include "Transform.h"

namespace TSFYP
{
	// Forward declarations
	struct Ray;

	struct IntersectionResult;

	// Interface that allows for polymorphism in ray intersection calculation
	class IShape
	{
	public:
		IShape()
		: transform()
		{}
		virtual ~IShape() {}

		virtual bool Intersects(const Ray& ray, const Transform& transform, IntersectionResult* result) const = 0;

	protected:
		Transform transform;
	};
}