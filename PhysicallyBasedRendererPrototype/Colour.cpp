#include "Colour.h"

#include <algorithm>

namespace TSFYP
{
	Colour::Colour()
		: r(0.0f)
		, g(0.0f)
		, b(0.0f)
	{}

	Colour::Colour(const float _r, const float _g, const float _b)
		: r(_r)
		, g(_g)
		, b(_b)
	{}

	Colour operator+(const Colour& lhs, const Colour& rhs)
	{
		float r = (lhs.r + rhs.r) / 2.0f;
		float g = (lhs.g + rhs.g) / 2.0f;
		float b = (lhs.b + rhs.b) / 2.0f;

		return Colour(r, g, b);
	}

	Colour& operator+=(Colour& lhs, const Colour& rhs)
	{
		lhs.r = (lhs.r + rhs.r) / 2.0f;
		lhs.g = (lhs.g + rhs.g) / 2.0f;
		lhs.b = (lhs.b + rhs.b) / 2.0f;

		return lhs;
	}

	const Colour Colour::Black = Colour(0.0f, 0.0f, 0.0f);
	const Colour Colour::Grey = Colour(0.5f, 0.5f, 0.5f);
	const Colour Colour::White = Colour(1.0f, 1.0f, 1.0f);

	const Colour Colour::Red = Colour(1.0f, 0.0f, 0.0f);
	const Colour Colour::Green = Colour(0.0f, 1.0f, 0.0f);
	const Colour Colour::Blue = Colour(0.0f, 0.0f, 1.0f);

	const Colour Colour::Cyan = Colour(0.0f, 1.0f, 1.0f);
	const Colour Colour::Magenta = Colour(1.0f, 0.0f, 1.0f);
	const Colour Colour::Yellow = Colour(1.0f, 1.0f, 0.0f);

	const Colour Colour::Orange = Colour(1.0f, 0.5f, 0.0f);
	const Colour Colour::Pink = Colour(1.0f, 0.0f, 0.5f);
	const Colour Colour::Lime = Colour(0.5f, 1.0f, 0.0f);
	const Colour Colour::Teal = Colour(0.0f, 1.0f, 0.5f);
	const Colour Colour::Purple = Colour(0.5f, 0.0f, 1.0f);
	const Colour Colour::LightBlue = Colour(0.0f, 0.5f, 1.0f);
}