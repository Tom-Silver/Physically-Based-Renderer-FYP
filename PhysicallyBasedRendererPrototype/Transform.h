#pragma once

#include "Vector3.h"

namespace TSFYP
{
	class Transform
	{
	public:
		Transform()
			: pos(Vector3())
			, rotation(Vector3())
			, scale(Vector3())
		{}
		Transform(const Vector3& _pos, const Vector3& _rotation, const Vector3& _scale)
			: pos(_pos)
			, rotation(_rotation)
			, scale(_scale)
		{}

		Vector3 pos;
		Vector3 rotation;
		Vector3 scale;

	private:

	};
}