#pragma once

// External includes
#include <glm/glm.hpp>

namespace TSFYP
{
	struct Scene;
	struct SceneObject;
	struct Shader;
	struct Window;

	// Handles the rendering of the final image
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		bool Initialise(Window* windowPtr, Scene* scene);

		void Render();

		void CreateGui();

		void SetClearColour(const glm::vec3& colour) { mClearColour = colour; }

	private:
		void RenderSceneObject(SceneObject* sceneObject, Scene* scene);
		void RenderSkybox(Scene* scene);

		void SetLightShaderData(Scene* scene, Shader* shader);

		Scene* pScene;
		Shader* mSkyboxShader;
		glm::vec3 mClearColour;
		Window* pWindow;
	};
}