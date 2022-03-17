#pragma once

#include "Colour.h"

namespace TSFYP
{
	struct Material;
	struct Mesh;
	class Transform;

	// Defines a 3D visible object in the scene to be rendered
	struct SceneObject
	{
		void CreateGui();

		Mesh* mesh;
		Material* material;
		Transform* transform;
	};
}