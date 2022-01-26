#pragma once

// Parent includes
#include "IShape.h"

namespace TSFYP
{
	// Implements a simple 3D sphere for ray intersection
	class Sphere :
		public IShape
	{
	public:
		Sphere(const float _radius)
			: radius(_radius)
			, radiusSq(_radius* _radius)
		{}
		virtual ~Sphere() {}

		virtual bool Intersects(const Ray& ray, const Transform& transform, IntersectionResult* result) const override;

	private:
		float radius, radiusSq;
	};
}