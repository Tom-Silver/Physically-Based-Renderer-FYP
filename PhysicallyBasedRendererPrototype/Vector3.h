#pragma once

namespace TSFYP
{
	struct Point3;

	// Defines a 3D directional vector
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
		explicit Vector3(const Point3& point);

		float x, y, z;
	};

	// Defines a point in 3D space
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
		explicit Point3(const Vector3& vec);

		float x, y, z;
	};

	Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
	Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
	Vector3 operator*(const Vector3& lhs, const float rhs);
	Vector3 operator*(const Vector3& lhs, const Vector3& rhs);
	Vector3 operator/(const Vector3& lhs, const float rhs);
	Vector3 operator/(const float lhs, const Vector3& rhs);

	Vector3& operator+=(Vector3& lhs, const Vector3& rhs);
	Vector3& operator-=(Vector3& lhs, const Vector3& rhs);
	Vector3& operator*=(Vector3& lhs, const float rhs);
	Vector3& operator*=(Vector3& lhs, const Vector3& rhs);
	Vector3& operator/=(Vector3& lhs, const float rhs);
}