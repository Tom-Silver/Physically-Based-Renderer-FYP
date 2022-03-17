#pragma once

// External includes
#include <string>
#include <vector>

// Internal includes
#include "Texture2D.h"
#include "Uniform.h"

namespace TSFYP
{
	class Shader;

	struct Material
	{
		void CreateGui();

		Shader* shader;
		std::vector<Texture2D> textures; // While also uniforms, textures get special treatment
		std::vector<IUniform*> uniforms; // Stores all uniforms of shader for dynamic access
	};
}