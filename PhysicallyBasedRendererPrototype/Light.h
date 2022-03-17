#pragma once

// External includes
#include <glm/glm.hpp>

// Internal includes
#include "Colour.h"

namespace TSFYP
{
	// Defines a coloured light in world space
	class Light
	{
	public:
		Light(const glm::vec3& pos, const glm::vec3& emissionColour);
		~Light();

		void CreateGui();

		const glm::vec3& pos() const { return mPos; }
		const glm::vec3& emittedColour() const { return mEmissionColour; }

	private:
		glm::vec3 mPos;
		glm::vec3 mEmissionColour;
	};
}