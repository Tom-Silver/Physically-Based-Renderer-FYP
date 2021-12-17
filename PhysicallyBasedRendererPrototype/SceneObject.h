#pragma once

#include "Colour.h"

namespace TSFYP
{
	class IShape;
	class Transform;

	struct SceneObject
	{
		Colour colour = Colour::Green;
		IShape* shape;
		Transform* transform;
	};
}