#include "Light.h"

namespace TSFYP
{
	Light::Light(const Point3& pos, const Colour& emissionColour)
		: mPos(pos)
		, mEmissionColour(emissionColour)
	{}

	Light::~Light()
	{}
}