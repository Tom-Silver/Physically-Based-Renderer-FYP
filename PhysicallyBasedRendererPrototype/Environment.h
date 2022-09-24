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
			, mLights()
		{}

		std::string mName;
		Texture2D mBackground;
		Texture2D mIrradianceMap;
		Texture2D mPrefilterMap;
		Texture2D mBRDFLUT;
		std::vector<ILight*> mLights;
	};
}