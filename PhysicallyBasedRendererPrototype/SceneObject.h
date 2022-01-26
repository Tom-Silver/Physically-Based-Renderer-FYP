#pragma once

#include "Colour.h"

namespace TSFYP
{
	class IShape;
	class Transform;

	// Defines a 3D visible object in the scene to be rendered
	struct SceneObject
	{
		Colour colour = Colour::Green;
		IShape* shape;
		Transform* transform;
	};
}