#pragma once

#include "Vector3.h"

namespace TSFYP
{
	// Defines a ray that traverses infinitely in a direction from an origin
	struct Ray
	{
		Ray()
		{}
		Ray(const Point3& _origin, const Vector3& _dir)
			: origin(_origin)
			, dir(_dir)
		{}

		Point3 origin;
		Vector3 dir;
	};
}