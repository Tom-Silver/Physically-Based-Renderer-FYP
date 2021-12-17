#include "Vector3.h"

namespace TSFYP
{
	Vector3::Vector3(const Point3& point)
		: x(point.x)
		, y(point.y)
		, z(point.z)
	{}
	Point3::Point3(const Vector3& vec)
		: x(vec.x)
		, y(vec.z)
		, z(vec.z)
	{}

	Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	Vector3 operator*(const Vector3& lhs, const float rhs)
	{
		return Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}

	Vector3 operator/(const Vector3& lhs, const float rhs)
	{
		return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}

	Vector3& operator+=(Vector3& lhs, const Vector3& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;

		return lhs;
	}

	Vector3& operator-=(Vector3& lhs, const Vector3& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;

		return lhs;
	}

	Vector3& operator*=(Vector3& lhs, const float rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;

		return lhs;
	}

	Vector3& operator/=(Vector3& lhs, const float rhs)
	{
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.z /= rhs;

		return lhs;
	}
}