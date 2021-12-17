#pragma once

namespace TSFYP
{
	struct Point3;

	struct Vector3
	{
		Vector3()
			: x(0.0f)
			, y(0.0f)
			, z(0.0f)
		{}
		Vector3(const float _x, const float _y, const float _z)
			: x(_x)
			, y(_y)
			, z(_z)
		{}
		Vector3(const Point3& point);

		float x, y, z;
	};

	struct Point3
	{
		Point3()
			: x(0.0f)
			, y(0.0f)
			, z(0.0f)
		{}
		Point3(const float _x, const float _y, const float _z)
			: x(_x)
			, y(_y)
			, z(_z)
		{}
		Point3(const Vector3& vec);

		float x, y, z;
	};

	Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
	Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
	Vector3 operator*(const Vector3& lhs, const float rhs);
	Vector3 operator/(const Vector3& lhs, const float rhs);

	Vector3& operator+=(Vector3& lhs, const Vector3& rhs);
	Vector3& operator-=(Vector3& lhs, const Vector3& rhs);
	Vector3& operator*=(Vector3& lhs, const float rhs);
	Vector3& operator/=(Vector3& lhs, const float rhs);
}