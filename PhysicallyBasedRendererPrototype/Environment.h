#pragma once

// External includes
#include <string>

// Internal includes
#include "Light.h"
#include "Texture2D.h"

namespace TSFYP
{
	struct Environment
	{
		Environment()
			: mName("")
			, mBackground()
			, mIrradianceMap()
			, mSpecularMap()
			, mLights()
		{}

		std::string mName;
		Texture2D mBackground;
		Texture2D mIrradianceMap;
		Texture2D mSpecularMap;
		std::vector<ILight*> mLights;
	};
}