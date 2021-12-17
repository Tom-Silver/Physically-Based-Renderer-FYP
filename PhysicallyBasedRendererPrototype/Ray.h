#pragma once

#include "Vector3.h"

namespace TSFYP
{
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