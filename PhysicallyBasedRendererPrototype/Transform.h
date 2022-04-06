#pragma once

// External includes
#include <glm/glm.hpp>

namespace TSFYP
{
	// Stores position, rotation and scale data and provides world matrix
	class Transform
	{
	public:
		Transform();
		Transform(const glm::vec3& pos, const float rotationAngle, const glm::vec3& rotationAxis, const glm::vec3& scale);

		// Accessors //
		const glm::vec3& pos() const { return mPos; }
		const float rotationAngle() const { return mRotationAngle; }
		const glm::vec3& rotationAxis() const { return mRotationAxis; }
		const glm::vec3& scale() const { return mScale; }

		const glm::mat4& world() { return mWorldDirty ? RecalculateWorld() : mWorld; }

		// Mutators //
		void SetPos(const glm::vec3& pos) { mPos = pos; mWorldDirty = true; }
		void SetRotationAngle(const float rotationAngle) { mRotationAngle = rotationAngle; mWorldDirty = true; }
		void SetRotationAxis(const glm::vec3& rotationAxis) { mRotationAxis = rotationAxis; mWorldDirty = true; }
		void SetScale(const glm::vec3& scale) { mScale = scale; mWorldDirty = true; }

	private:
		const glm::mat4& RecalculateWorld();

		// World matrix params
		glm::vec3 mPos;
		float mRotationAngle;
		glm::vec3 mRotationAxis;
		glm::vec3 mScale;

		glm::mat4 mWorld;
		bool mWorldDirty;
	};
}