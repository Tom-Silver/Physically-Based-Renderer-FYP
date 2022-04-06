#include "Light.h"

// External includes
#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>
#include <string>

// Internal includes
#include "Shader.h"

namespace TSFYP
{
	ILight::ILight(const glm::vec3& emissionColour, const LightType lightType)
		: mEmissionColour(emissionColour)
		, mLightType(lightType)
	{}

	PointLight::PointLight(const glm::vec3& pos, const glm::vec3& emissionColour)
		: ILight(emissionColour, LightType::POINT)
		, mPos(pos)
	{}

	void PointLight::Set(Shader* shader, int i)
	{
		shader->SetUniform("pointLights[" + std::to_string(i) + "].position", mPos);
		shader->SetUniform("pointLights[" + std::to_string(i) + "].emittedColour", mEmissionColour);
	}

	void PointLight::CreateGui()
	{
		ImGui::ColorPicker3("Emitted colour", glm::value_ptr(mEmissionColour));
		ImGui::DragFloat3("Light pos", glm::value_ptr(mPos));
	}

	DirectionalLight::DirectionalLight(const glm::vec3& dir, const glm::vec3& emissionColour)
		: ILight(emissionColour, LightType::DIRECTIONAL)
		, mDir(dir)
	{}

	void DirectionalLight::Set(Shader* shader, int i)
	{
		shader->SetUniform("directionalLights[" + std::to_string(i) + "].direction", mDir);
		shader->SetUniform("directionalLights[" + std::to_string(i) + "].emittedColour", mEmissionColour);
	}

	void DirectionalLight::CreateGui()
	{
		ImGui::ColorPicker3("Emitted colour", glm::value_ptr(mEmissionColour));
		ImGui::DragFloat3("Light dir", glm::value_ptr(mDir));
	}
}