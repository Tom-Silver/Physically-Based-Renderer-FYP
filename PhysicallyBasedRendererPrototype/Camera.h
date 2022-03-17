#pragma once

// External includes
#include <glm/glm.hpp>

namespace TSFYP
{
	// Stores camera properties and provides view/projection matrices
	class Camera
	{
	public:
		Camera();
		Camera(const glm::vec3 pos, const glm::vec3 at, const glm::vec3 up, const float fov, const glm::vec2& viewportSize, float near, float far);

		// Call to display camera information
		void CreateGui();

		// Accessors //
		const glm::vec3& pos() const { return mPos; }
		const glm::vec3& up() const { return mUp; }
		const glm::vec3& at() const { return mAt; }
		const glm::vec3& forward() { if (mViewDirty) { RecalculateView(); } return glm::normalize(-glm::transpose(mView)[2]); }
		const glm::vec3& right() { if (mViewDirty) { RecalculateView(); } return glm::normalize(-glm::transpose(mView)[0]); }

		const float fov() const { return mFov; }

		const glm::mat4& view() { return mViewDirty ? RecalculateView() : mView; }
		const glm::mat4& projection() { return mProjectionDirty ? RecalculateProjection() : mProjection; }

		// Mutators //
		void SetPos(const glm::vec3& pos) { mPos = pos; mViewDirty = true; }
		void SetUp(const glm::vec3& up) { mUp = up; mViewDirty = true; }
		void SetAt(const glm::vec3& at) { mAt = at; mViewDirty = true; }

		void SetFov(const float fov) { mFov = fov; mProjectionDirty = true; }
		void SetViewportSize(const glm::vec2& viewportSize) { mViewportSize = viewportSize; mProjectionDirty = true; }

	private:
		const glm::mat4& RecalculateView();
		const glm::mat4& RecalculateProjection();

		// View matrix params
		glm::vec3 mPos;
		glm::vec3 mUp;
		glm::vec3 mAt;
		glm::mat4 mView;
		bool mViewDirty;

		// Projection matrix params
		float mFov;
		glm::vec2 mViewportSize;
		float mNear;
		float mFar;
		glm::mat4 mProjection;
		bool mProjectionDirty;
	};
}