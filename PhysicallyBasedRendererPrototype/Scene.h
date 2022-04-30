#pragma once

// External includes
#include <vector>

// Internal includes
#include "Camera.h"
#include "Cube.h"
#include "Environment.h"
#include "Light.h"
#include "SceneObject.h"
#include "Texture2D.h"

namespace TSFYP
{
	// Stores the 3D scene to be rendered, including the material sphere, lights, and camera
	struct Scene
	{
		Scene();

		void Update(float deltaTime);

		void CreateGui();

		Camera mCamera;
		SceneObject mObject;

		// For IBL
		Cube* mSkyboxMesh;
		Environment* mEnvironment;

		float mObjectAutoRotationSpeed;
	};
}