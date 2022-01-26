#pragma once

#include "Colour.h"
#include "Vector3.h"

namespace TSFYP
{
	// Defines a coloured light in world space
	class Light
	{
	public:
		Light(const Point3& pos, const Colour& emissionColour);
		~Light();

		Point3 pos() const { return mPos; }
		Colour emittedColour() const { return mEmissionColour; }

	private:
		Point3 mPos;
		Colour mEmissionColour;
	};
}