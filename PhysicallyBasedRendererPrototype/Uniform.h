#pragma once

// External includes
#include <string>

// Internal includes
#include "Shader.h"

namespace TSFYP
{
	// Abstract class representing a uniform variable.
	// Uses a templated child class so that this one can be used to store ambiguous uniforms in one container
	struct IUniform
	{
		std::string name;

		virtual void Set(Shader* shader) = 0;
		virtual void CreateGui() = 0;
	};

	// Defines the data portion of the uniform.
	// Acts as the base class for the usable implementations
	template <typename T>
	struct Uniform
		: public IUniform
	{
		T data;

		virtual void Set(Shader* shader) override
		{
			shader->SetUniform("material." + name, data);
		}
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