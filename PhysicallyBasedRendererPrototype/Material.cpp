#include "Material.h"

namespace TSFYP
{
	void Material::CreateGui()
	{
		for (IUniform* uniform : uniforms)
		{
			uniform->CreateGui();
		}
	}
}