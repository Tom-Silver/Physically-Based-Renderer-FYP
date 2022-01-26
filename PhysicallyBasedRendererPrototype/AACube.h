#pragma once

// Parent includes
#include "IShape.h"

#include <assert.h>

namespace TSFYP
{
	// Defines a 3D axis-aligned cube for ray intersection
	class AACube :
		public IShape
	{
	public:
		AACube(const Vector3& size)
		{
			assert(size.x > 0.0f && size.y > 0.0f && size.z > 0.0f); // Negative size is illogical and breaks the min/max bounds

			Vector3 halfSize = size / 2.0f;
			bounds[0] = Vector3(0.0f - halfSize.x, 0.0f - halfSize.y, 0.0f - halfSize.z);
			bounds[1] = Vector3(0.0f + halfSize.x, 0.0f + halfSize.y, 0.0f + halfSize.z);
		}
		virtual ~AACube() {}

		virtual bool Intersects(const Ray& ray, const Transform& transform, IntersectionResult* result) const override;

	private:
		Vector3 bounds[2];
	};
}