#include "Light.h"

// External includes
#include <ImGui/imgui.h>
#include <string>

namespace TSFYP
{
	Light::Light(const glm::vec3& pos, const glm::vec3& emissionColour)
		: mPos(pos)
		, mEmissionColour(emissionColour)
	{}

	Light::~Light()
	{}

	void Light::CreateGui()
	{
		// Pos
		{
			std::string text = "Pos: " + std::to_string(mPos.x) + ", " + std::to_string(mPos.y) + ", " + std::to_string(mPos.z);
			ImGui::Text(text.c_str());
		}

		// Emitted colour
		{
			std::string text = "Emitted colour: " + std::to_string(mEmissionColour.x) + ", " + std::to_string(mEmissionColour.y) + ", " + std::to_string(mEmissionColour.z);
			ImGui::Text(text.c_str());
		}
	}
}