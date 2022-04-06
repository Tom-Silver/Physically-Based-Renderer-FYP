#include "Renderer.h"

// External includes
#include <algorithm>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui_impl_opengl3.h>
#include <iostream>

// Internal includes
#include "ImGui/imgui.h"
#include "Material.h"
#include "Mesh.h"
#include "Scene.h"
#include "Shader.h"
#include "Transform.h"
#include "Window.h"

// Preprocessor definitions
#define PI 3.141592653589793f

namespace TSFYP
{
	void SetMaterialUniforms(const Material* material);

	void CheckGLErrors();

	Renderer::Renderer()
		: mClearColour(glm::vec3(0.0f))
		, pWindow(nullptr)
		, pScene(nullptr)
	{}

	// The pointer members are not the responsibility of Renderer, so there is nothing to delete
	Renderer::~Renderer()
	{}

	void Renderer::Render()
	{
		// Begin render by clearing buffer with set clear colour
		glClearColor(mClearColour.r, mClearColour.g, mClearColour.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderSceneObject(&pScene->mObject, pScene);

		// Render ImGui after scene so it appears above it
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Finally, swap buffers
		glfwSwapBuffers(pWindow->glfwWindow);
	}

	void Renderer::CreateGui()
	{
		ImGui::ColorPicker3("Clear colour", glm::value_ptr(mClearColour));
	}

	bool Renderer::Initialise(Window* windowPtr, Scene* scene)
	{
		if (!windowPtr) { return false; }
		pWindow = windowPtr;
		
		if (!scene) { return false; }
		pScene = scene;

		return true;
	}

	void Renderer::RenderSceneObject(SceneObject* sceneObject, Scene* scene)
	{
		// Bind shader
		Shader* shader = sceneObject->material->shader;
		shader->Use();
		
		// Set uniform matrices
		glm::mat4 projection = scene->mCamera.projection();
		shader->SetUniform("projection", projection);

		glm::mat4 view = scene->mCamera.view();
		shader->SetUniform("view", view);

		glm::mat4 world = sceneObject->transform->world();
		shader->SetUniform("world", world);

		SetMaterialUniforms(sceneObject->material);
		SetLightShaderData(scene, shader);

		// Set uniform textures
		unsigned int albedoNo = 0;
		unsigned int normalNo = 0;
		unsigned int metallicNo = 0;
		unsigned int roughnessNo = 0;
		unsigned int aoNo = 0;
		for (unsigned int i = 0; i < sceneObject->material->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			Texture2D texture = sceneObject->material->textures[i];

			std::string uniformName;
			switch (texture.textureType)
			{
			case Texture2D::TextureType::ALBEDO:
				uniformName = "albedo" + std::to_string(albedoNo++);
				break;
			case Texture2D::TextureType::NORMAL:
				uniformName = "normal" + std::to_string(normalNo++);
				break;
			case Texture2D::TextureType::METALLIC:
				uniformName = "metallic" + std::to_string(metallicNo++);
				break;
			case Texture2D::TextureType::ROUGHNESS:
				uniformName = "roughness" + std::to_string(roughnessNo++);
				break;
			case Texture2D::TextureType::AO:
				uniformName = "ao" + std::to_string(aoNo++);
				break;
			}

			shader->SetUniform("material." + uniformName, (int)i);

			glBindTexture(GL_TEXTURE_2D, texture.id);
		}

		// Bind VAO and draw
		glBindVertexArray(sceneObject->mesh->vao);
		glDrawElements(GL_TRIANGLE_STRIP, sceneObject->mesh->indexCount, GL_UNSIGNED_INT, 0);

		// Unbind bound objects
		glBindVertexArray(0);
		shader->Unuse();
		glActiveTexture(GL_TEXTURE0);
	}

	void Renderer::SetLightShaderData(Scene* scene, Shader* shader)
	{
		unsigned int noLights = scene->mLights.size();
		shader->SetUniform("noLights", noLights);

		glm::vec3 camPos = scene->mCamera.pos();
		shader->SetUniform("camPos", camPos);

		unsigned int pointNo = 0;
		unsigned int directionalNo = 0;
		unsigned int spotNo = 0;
		for (unsigned int i = 0; i < noLights; i++)
		{
			ILight* light = scene->mLights[i];
			switch (light->lightType())
			{
			case ILight::LightType::POINT:
				light->Set(shader, pointNo++);
				break;
			case ILight::LightType::DIRECTIONAL:
				light->Set(shader, directionalNo++);
				break;
			case ILight::LightType::SPOT:
				light->Set(shader, spotNo++);
				break;
			}
		}

		shader->SetUniform("noPoint", pointNo);
		shader->SetUniform("noDirectional", directionalNo);
		shader->SetUniform("noSpot", spotNo);
	}

	void SetMaterialUniforms(const Material* material)
	{
		for (unsigned int i = 0; i < material->uniforms.size(); i++)
		{
			material->uniforms[i]->Set(material->shader);
		}
	}

	void CheckGLErrors()
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "GL error! Code: " << error << std::endl;
		}
	}
}