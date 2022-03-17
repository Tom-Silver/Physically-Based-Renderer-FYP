#pragma once

// External includes
#include <vector>

// Internal includes
#include "Camera.h"
#include "Light.h"
#include "SceneObject.h"

namespace TSFYP
{
	// Stores the 3D scene to be rendered, including the material sphere, lights, and camera
	struct Scene
	{
		void CreateGui();

		Camera mCamera;
		SceneObject mObject;
		std::vector<Light> mLights; // public to allow access to loop through lights during rendering
	};
}