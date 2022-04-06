#pragma once

// External includes
#include <string>

namespace TSFYP
{
	struct Material;

	Material* LoadMaterial(std::string name);
}