#include "Uniform.h"

// External includes
#include <ImGui/imgui.h>

namespace TSFYP
{
	void IntUniform::CreateGui()
	{
		ImGui::DragInt(name.c_str(), &data);
	}

	void UintUniform::CreateGui()
	{
		ImGui::DragInt(name.c_str(), (int*)&data, 1.0f, 0);
	}

	void BoolUniform::CreateGui()
	{
		ImGui::Checkbox(name.c_str(), &data);
	}

	void FloatUniform::CreateGui()
	{
		ImGui::DragFloat(name.c_str(), &data);
	}
}