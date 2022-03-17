#include "Transform.h"

// External includes
#include <glm/gtc/matrix_transform.hpp>

namespace TSFYP
{
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