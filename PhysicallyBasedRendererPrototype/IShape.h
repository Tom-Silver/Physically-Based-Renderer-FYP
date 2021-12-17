#pragma once

#include "Transform.h"

namespace TSFYP
{
	// Forward declarations
	struct Ray;

	struct IntersectionResult;

	class IShape
	{
	public:
		IShape()
		: transform()
		{}
		virtual ~IShape() {}

		virtual bool Intersects(const Ray& ray, IntersectionResult* result) = 0;

	protected:
		Transform transform;
	};
}