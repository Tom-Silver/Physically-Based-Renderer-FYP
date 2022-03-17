#include "Scene.h"

// External includes
#include <limits>

// Internal includes
#include "Sphere.h"

namespace TSFYP
{
	void Scene::CreateGui()
	{
		mCamera.CreateGui();
		mObject.CreateGui();
		for (Light& light : mLights)
		{
			light.CreateGui();
		}
	}
}