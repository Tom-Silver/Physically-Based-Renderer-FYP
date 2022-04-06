#pragma once

// External includes
#include <glm/glm.hpp>

namespace TSFYP
{
	struct Shader;

	// Defines a light emitter of ambiguous type, used for ambiguous storage
	class ILight
	{
	public:
		enum class LightType
		{
			POINT,
			DIRECTIONAL,
			SPOT
		};

		ILight(const glm::vec3& emissionColour, const LightType lightType);

		virtual void Set(Shader* shader, int i) = 0;
		virtual void CreateGui() = 0;

		const glm::vec3& emittedColour() const { return mEmissionColour; }
		const LightType lightType() const { return mLightType; }

	protected:
		glm::vec3 mEmissionColour;
		const LightType mLightType;
	};

	// Defines a light emitting outward from a point in world space
	class PointLight
		: public ILight
	{
	public:
		PointLight(const glm::vec3& pos, const glm::vec3& emissionColour);

		virtual void Set(Shader* shader, int i) override;
		virtual void CreateGui() override;

		const glm::vec3& pos() const { return mPos; }

	private:
		glm::vec3 mPos;
	};

	// Defines a light emitted in a single direction infinitely
	class DirectionalLight
		: public ILight
	{
	public:
		DirectionalLight(const glm::vec3& dir, const glm::vec3& emissionColour);

		virtual void Set(Shader* shader, int i) override;
		virtual void CreateGui() override;

		const glm::vec3& dir() const { return mDir; }

	private:
		glm::vec3 mDir;
	};
}