#pragma once

namespace TSFYP
{
	// Defines a camera. Will later be expanded to define it in 3D space to allow for positional ray generation
	class Camera
	{
	public:
		Camera(const float fov);

		float fov() const { return mFov; }

	private:
		float mFov;
	};
}