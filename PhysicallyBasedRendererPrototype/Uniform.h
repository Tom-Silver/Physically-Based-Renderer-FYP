#pragma once

// External includes
#include <string>

// Internal includes
#include "Shader.h"

namespace TSFYP
{
	struct IUniform
	{
		std::string name;

		virtual void Set(Shader* shader) = 0;
		virtual void CreateGui() = 0;
	};

	template <typename T>
	struct Uniform
		: public IUniform
	{
		T data;

		virtual void Set(Shader* shader) override
		{
			shader->SetUniform("material." + name, data);
		}

		virtual void CreateGui() = 0;
	};

	struct IntUniform
		: Uniform<int>
	{
		virtual void CreateGui() override;
	};

	struct UintUniform
		: Uniform<unsigned int>
	{
		virtual void CreateGui() override;
	};

	struct BoolUniform
		: Uniform<bool>
	{
		virtual void CreateGui() override;
	};

	struct FloatUniform
		: Uniform<float>
	{
		virtual void CreateGui() override;
	};
}