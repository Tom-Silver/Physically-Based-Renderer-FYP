#pragma once

// External includes
#include <math.h>

// Internal includes
#include "SceneObject.h"
#include "Vector2.h"
#include "Vector3.h"

namespace TSFYP
{
	struct IntersectionResult
	{
		float t;
		Point3 intersectionPoint;
		Vector3 intersectionNormal;
		const SceneObject* intersectedObject;
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
	
	inline float LengthSq(const Vector3& vec)
	{
		return (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
	}

	inline Vector3& Normalise(Vector3& vec)
	{
		float length = sqrtf(LengthSq(vec));
		if (length > 0)
		{
			return vec *= (1.0f / length);
		}

		return vec = Vector3();
	}

	inline const Vector3& Normalise(const Vector3& vec)
	{
		float lengthSq = LengthSq(vec);
		if (lengthSq > 0)
		{
			return vec * (1.0f / sqrtf(lengthSq));
		}

		return Vector3();
	}
}