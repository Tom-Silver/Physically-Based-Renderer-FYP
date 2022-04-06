#include "Transform.h"

// External includes
#include <glm/gtc/matrix_transform.hpp>

namespace TSFYP
{
	Transform::Transform()
		: mPos(0.0f, 0.0f, 0.0f)
		, mRotationAngle(0.0f)
		, mRotationAxis(0.0f, 1.0f, 0.0f)
		, mScale(1.0f, 1.0f, 1.0f)
		, mWorld(1.0f)
		, mWorldDirty(true)
	{}

	Transform::Transform(const glm::vec3& pos, const float rotationAngle, const glm::vec3& rotationAxis, const glm::vec3& scale)
		: mPos(pos)
		, mRotationAngle(rotationAngle)
		, mRotationAxis(rotationAxis)
		, mScale(scale)
		, mWorld(1.0f)
		, mWorldDirty(true)
	{}

	const glm::mat4& Transform::RecalculateWorld()
	{
		mWorld = glm::mat4(1.0f);
		mWorld = glm::translate(mWorld, mPos);
		mWorld = glm::rotate(mWorld, mRotationAngle, mRotationAxis);
		mWorld = glm::scale(mWorld, mScale);

		mWorldDirty = false;
		return mWorld;
	}
}