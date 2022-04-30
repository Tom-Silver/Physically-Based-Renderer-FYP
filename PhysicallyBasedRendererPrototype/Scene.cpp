#include "Scene.h"

// External includes
#include <ImGui/imgui.h>
#include <string>

// Internal includes
#include "Sphere.h"

namespace TSFYP
{
	Scene::Scene()
		: mCamera()
		, mObject()
		, mObjectAutoRotationSpeed(0.1f)
	{}

	void Scene::Update(float deltaTime)
	{
		float oldAngle = mObject.transform->rotationAngle();
		float newAngle = oldAngle + (mObjectAutoRotationSpeed * deltaTime);
		mObject.transform->SetRotationAngle(newAngle);
	}

	void Scene::CreateGui()
	{
		static ILight* currentLight = nullptr;
		static std::string previewName = "Light selection";

		mObject.CreateGui();

		if (ImGui::Begin("Environment Info"))
		{
			mCamera.CreateGui();

			ImGui::Separator();

			// Light selection combo box
			if (ImGui::BeginCombo("Lights", previewName.c_str()))
			{
				for (int i = 0; i < mEnvironment->mLights.size(); i++)
				{
					bool isSelected = (currentLight == mEnvironment->mLights[i]);
					if (ImGui::Selectable(std::to_string(i).c_str(), isSelected))
					{
						currentLight = mEnvironment->mLights[i];
						previewName = std::to_string(i);
					}
				}

				ImGui::EndCombo();
			}

			if (currentLight)
			{
				currentLight->CreateGui();
			}
			
			ImGui::End();
		}
	}
}