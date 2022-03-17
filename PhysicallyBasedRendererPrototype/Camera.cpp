#include "Camera.h"

// External includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>
#include <string>

namespace TSFYP
{
	Camera::Camera()
		: mPos(0.0f, 0.0f, 0.0f)
		, mAt(0.0f, 0.0f, 0.0f)
		, mUp(0.0f, 1.0f, 0.0f)
		, mView(1.0f)
		, mViewDirty(true)
		, mFov(45.0f)
		, mViewportSize(0.0f, 0.0f)
		, mNear(0.1f)
		, mFar(100.0f)
		, mProjection(1.0f)
		, mProjectionDirty(true)
	{}

	Camera::Camera(const glm::vec3 pos, const glm::vec3 at, const glm::vec3 up, const float fov, const glm::vec2& viewportSize, float near, float far)
		: mPos(pos)
		, mAt(at)
		, mUp(up)
		, mView(1.0f)
		, mViewDirty(true)
		, mFov(fov)
		, mViewportSize(viewportSize)
		, mNear(near)
		, mFar(far)
		, mProjection(1.0f)
		, mProjectionDirty(true)
	{}

	void Camera::CreateGui()
	{
		if (ImGui::Begin("Camera"))
		{
			// Pos
			{
				glm::vec3 pos = this->pos();
				std::string text = "Pos: " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z);
				ImGui::Text(text.c_str());
			}
			
			// At
			{
				glm::vec3 at = this->at();
				std::string text = "At: " + std::to_string(at.x) + ", " + std::to_string(at.y) + ", " + std::to_string(at.z);
				ImGui::Text(text.c_str());
			}

			// Up
			{
				glm::vec3 up = this->up();
				std::string text = "Up: " + std::to_string(up.x) + ", " + std::to_string(up.y) + ", " + std::to_string(up.z);
				ImGui::Text(text.c_str());
			}

			// Forward vector
			{
				glm::vec3 forward = this->forward();
				std::string text = "Forward: " + std::to_string(forward.x) + ", " + std::to_string(forward.y) + ", " + std::to_string(forward.z);
				ImGui::Text(text.c_str());
			}

			// Right vector
			{
				glm::vec3 right = this->right();
				std::string text = "Right: " + std::to_string(right.x) + ", " + std::to_string(right.y) + ", " + std::to_string(right.z);
				ImGui::Text(text.c_str());
			}
		}

		ImGui::End();
	}

	const glm::mat4& Camera::RecalculateView()
	{
		mView = glm::lookAt(mPos, mAt, mUp);
		mViewDirty = false;

		return mView;
	}

	const glm::mat4& Camera::RecalculateProjection()
	{
		mProjection = glm::perspective(mFov, mViewportSize.x / mViewportSize.y, mNear, mFar);
		mProjectionDirty = false;

		return mProjection;
	}
}