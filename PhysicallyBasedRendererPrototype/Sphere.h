#pragma once

// Parent includes
#include "IShape.h"

namespace TSFYP
{
	class Sphere :
		public IShape
	{
	public:
		Sphere(const float _radius)
			: radius(_radius)
			, radiusSq(_radius* _radius)
		{}
		virtual ~Sphere() {}

		virtual bool Intersects(const Ray& ray, IntersectionResult* result) override;

	private:
		float radius, radiusSq;
	};
}