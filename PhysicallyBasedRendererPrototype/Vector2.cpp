#include "Vector2.h"

namespace TSFYP
{
	Vector2::Vector2(const Point2& point)
		: x(point.x)
		, y(point.y)
	{}
	Point2::Point2(const Vector2& vec)
		: x(vec.x)
		, y(vec.y)
	{}

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	Vector2 operator*(const Vector2& lhs, const float rhs)
	{
		return Vector2(lhs.x * rhs, lhs.y * rhs);
	}

	Vector2 operator*(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	Vector2 operator/(const Vector2& lhs, const float rhs)
	{
		return Vector2(lhs.x / rhs, lhs.y / rhs);
	}

	Vector2 operator/(const float lhs, const Vector2& rhs)
	{
		return Vector2(lhs / rhs.x, lhs / rhs.y);
	}

	Vector2& operator+=(Vector2& lhs, const Vector2& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;

		return lhs;
	}

	Vector2& operator-=(Vector2& lhs, const Vector2& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;

		return lhs;
	}

	Vector2& operator*=(Vector2& lhs, const float rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;

		return lhs;
	}

	Vector2& operator*=(Vector2& lhs, const Vector2& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;

		return lhs;
	}

	Vector2& operator/=(Vector2& lhs, const float rhs)
	{
		lhs.x /= rhs;
		lhs.y /= rhs;

		return lhs;
	}
}