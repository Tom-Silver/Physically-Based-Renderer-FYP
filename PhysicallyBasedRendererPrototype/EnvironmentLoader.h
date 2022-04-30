#pragma once

// External includes
#include <string>

namespace TSFYP
{
	struct Environment;
	struct Window;

	Environment* LoadEnvironment(const std::string& name, Window* window);
}