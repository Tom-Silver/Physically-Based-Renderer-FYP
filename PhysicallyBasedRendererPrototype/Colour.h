#pragma once

namespace TSFYP
{
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
		
		float r, g, b;

		static const Colour Black;
		static const Colour White;
		static const Colour Red;
		static const Colour Green;
		static const Colour Blue;
	};
}