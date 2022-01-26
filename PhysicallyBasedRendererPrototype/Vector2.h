#pragma once

namespace TSFYP
{
	struct Point2;

	// Defines a 2D directional vector
	struct Vector2
	{
		Vector2()
			: x(0.0f)
			, y(0.0f)
		{}
		Vector2(const float _x, const float _y)
			: x(_x)
			, y(_y)
		{}
		explicit Vector2(const Point2& point);

		float x, y;
	};

	// Defines a point in 2D space
	struct Point2
	{
		Point2()
			: x(0.0f)
			, y(0.0f)
		{}
		Point2(const float _x, const float _y, const float _z)
			: x(_x)
			, y(_y)
		{}
		explicit Point2(const Vector2& vec);

		float x, y;
	};

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
	Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
	Vector2 operator*(const Vector2& lhs, const float rhs);
	Vector2 operator*(const Vector2& lhs, const Vector2& rhs);
	Vector2 operator/(const Vector2& lhs, const float rhs);
	Vector2 operator/(const float lhs, const Vector2& rhs);

	Vector2& operator+=(Vector2& lhs, const Vector2& rhs);
	Vector2& operator-=(Vector2& lhs, const Vector2& rhs);
	Vector2& operator*=(Vector2& lhs, const float rhs);
	Vector2& operator*=(Vector2& lhs, const Vector2& rhs);
	Vector2& operator/=(Vector2& lhs, const float rhs);
}