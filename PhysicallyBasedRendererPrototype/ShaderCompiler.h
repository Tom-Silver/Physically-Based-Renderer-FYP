#pragma once

// Internal includes
#include "Shader.h" // Usually we would forward declare and hide, but all includes of this file will also need to include Shader.h anyway

namespace TSFYP
{
	Shader* CreateShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
}