#pragma once

#include "Vector3.h"

namespace TSFYP
{
	// Stores an RGB colour and defines additive colour mixing operators
	struct Colour
	{
		Colour()
			: r(0.0f)
			, g(0.0f)
			, b(0.0f)
		{}
		Colour(const float _r, const float _g, const float _b)
			: r(_r)
			, g(_g)
			, b(_b)
		{}
		explicit Colour(const Vector3& vec);
		
		explicit operator Vector3() const { return Vector3(r, g, b); }

		float r, g, b;

		static const Colour Black;
		static const Colour Grey;
		static const Colour White;
		
		static const Colour Red;
		static const Colour Green;
		static const Colour Blue;

		static const Colour Cyan;
		static const Colour Magenta;
		static const Colour Yellow;

		static const Colour Orange;
		static const Colour Pink;
		static const Colour Lime;
		static const Colour Teal;
		static const Colour Purple;
		static const Colour LightBlue;
	};

	// Additive colour mixing
	Colour operator+(const Colour& lhs, const Colour& rhs);
	Colour& operator+=(Colour& lhs, const Colour& rhs);
}