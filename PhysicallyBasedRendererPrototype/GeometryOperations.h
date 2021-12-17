#pragma once

#include "SceneObject.h"
#include "Vector3.h"

namespace TSFYP
{
	struct IntersectionResult
	{
		float t;
		Point3 intersectionPoint;
		SceneObject* intersectedObject;
	};

	inline float Dot(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	inline Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
	{
		float crossX = lhs.y * rhs.z - lhs.z * rhs.y;
		float crossY = lhs.z * rhs.x - lhs.x * rhs.z;
		float crossZ = lhs.x * rhs.y - lhs.y * rhs.x;

		return Vector3(crossX, crossY, crossZ);
	}
}