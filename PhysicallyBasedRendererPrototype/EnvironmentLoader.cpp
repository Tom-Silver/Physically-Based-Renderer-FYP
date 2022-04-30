#include "EnvironmentLoader.h"

// External includes
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

// Internal includes
#include "Cube.h"
#include "Environment.h"
#include "Light.h"
#include "ShaderCompiler.h"
#include "Window.h"

using json = nlohmann::json;

namespace TSFYP
{
	Texture2D LoadTexture(const json& jTexture, const std::string& filePrefix);
	PointLight* LoadPointLight(const json& jLight, const glm::vec3& emittedColour);
	DirectionalLight* LoadDirectionalLight(const json& jLight, const glm::vec3& emittedColour);

	Texture2D CreateBackground(Texture2D equirectangularTexture, Window* window);
	Texture2D CreateIrradianceMap(Texture2D environment, Window* window);
	Texture2D CreateSpecularMap(Texture2D environment, Window* window);

	Environment* LoadEnvironment(const std::string& name, Window* window)
	{
		std::string prefix = "Resources/Environments/" + name + "/";

		// Open file
		std::ifstream inFile;
		{
			std::string fileName = "environment.json";
			inFile.open(prefix + fileName);

			if (inFile.bad())
			{
				return nullptr;
			}
		}

		// Parse JSON data
		json jFile;
		try
		{
			jFile = json::parse(inFile);
		}
		catch (nlohmann::json::parse_error& e)
		{
			std::cout << e.what() << std::endl;

			inFile.close();
			return nullptr;
		}

		inFile.close(); // We no longer need the file open

		Environment* newEnvironment = new Environment();

		if (jFile.contains("name"))
		{
			newEnvironment->mName = jFile["name"];
		}

		if (jFile.contains("background"))
		{
			Texture2D equirectangularTexture = LoadTexture(jFile["background"], prefix);
			newEnvironment->mBackground = CreateBackground(equirectangularTexture, window);
		}
		else
		{ // Background texture is required
			delete newEnvironment;
			return nullptr;
		}

		if (jFile.contains("irradiance"))
		{
			newEnvironment->mIrradianceMap = LoadTexture(jFile["irradiance"], prefix);
		}
		else
		{ // If we don't have a pre-made irradiance map, we can calculate one
			newEnvironment->mIrradianceMap = CreateIrradianceMap(newEnvironment->mBackground, window);
		}

		if (jFile.contains("specular"))
		{
			newEnvironment->mSpecularMap = LoadTexture(jFile["specular"], prefix);
		}
		else
		{ // If we don't have a pre-made specular map, we can calculate one
			newEnvironment->mSpecularMap = CreateSpecularMap(newEnvironment->mBackground, window);
		}

		if (jFile.contains("lights"))
		{
			for (int i = 0; i < jFile["lights"].size(); i++)
			{
				json jLight = jFile["lights"][i];

				ILight::LightType type(ILight::LightType::POINT);
				if (jLight.contains("type"))
				{
					type = (ILight::LightType)jLight["type"];
				}

				glm::vec3 emittedColour(1.0f);
				if (jLight.contains("emittedColour"))
				{
					json jColour = jLight["emittedColour"];
					
					emittedColour.x = jColour["r"];
					emittedColour.y = jColour["g"];
					emittedColour.z = jColour["b"];
				}

				ILight* newLight;
				switch (type)
				{
				case ILight::LightType::POINT:
					newLight = LoadPointLight(jLight, emittedColour);
					break;
				case ILight::LightType::DIRECTIONAL:
					newLight = LoadDirectionalLight(jLight, emittedColour);
					break;
				case ILight::LightType::SPOT:
					// Not currently supported
					break;
				}

				newEnvironment->mLights.emplace_back(newLight);
			}
		}

		return newEnvironment;
	}

	Texture2D LoadTexture(const json& jTexture, const std::string& filePrefix)
	{
		std::string name = "";
		std::string filePath = filePrefix;
		Texture2D::TextureType type(Texture2D::TextureType::ALBEDO);

		if (jTexture.contains("fileName"))
		{
			std::string fileName = jTexture["fileName"];
			filePath += fileName;
		}
		else
		{
			return Texture2D(); // Need t be able to load a file
		}

		if (jTexture.contains("name"))
		{
			name = jTexture["name"];
		}

		if (jTexture.contains("type"))
		{
			type = (Texture2D::TextureType)jTexture["type"];
		}

		return CreateTexture2DUInt(name, type, filePath);
	}

	PointLight* LoadPointLight(const json& jLight, const glm::vec3& emittedColour)
	{
		glm::vec3 pos(0.0f);
		if (jLight.contains("pos"))
		{
			json jPos = jLight["pos"];
			pos.x = jPos["x"];
			pos.y = jPos["y"];
			pos.z = jPos["z"];
		}

		return new PointLight(pos, emittedColour);
	}

	DirectionalLight* LoadDirectionalLight(const json& jLight, const glm::vec3& emittedColour)
	{
		glm::vec3 dir(0.0f);
		if (jLight.contains("dir"))
		{
			json jDir = jLight["dir"];
			dir.x = jDir["x"];
			dir.y = jDir["y"];
			dir.z = jDir["z"];
		}

		return new DirectionalLight(dir, emittedColour);
	}

	Texture2D CreateBackground(Texture2D equirectangularTexture, Window* window)
	{
		Cube* renderCube = CreateCube();

		Texture2D environmentMap = CreateEmptyCubemap("environmentcubemap", 512, 512);

		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		unsigned int captureFBO;
		glGenFramebuffers(1, &captureFBO);

		Shader* equirectangularToCubemapShader = CreateShader("equirectangularToCubemap", "Resources/Shaders/equirectangularToCubemapVS.glsl", "Resources/Shaders/equirectangularToCubemapFS.glsl");
		equirectangularToCubemapShader->Use();
		equirectangularToCubemapShader->SetUniform("equirectangularMap", 0);
		equirectangularToCubemapShader->SetUniform("projection", captureProjection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, equirectangularTexture.id);

		glViewport(0, 0, 512, 512); // Set viewport to dimensions of the capture (512x512)
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		for (unsigned int i = 0; i < 6; i++)
		{
			equirectangularToCubemapShader->SetUniform("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, environmentMap.id, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindVertexArray(renderCube->vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Clean up
		glViewport(0, 0, window->width, window->height); // Set viewport back to screen dimensions
		delete renderCube;
		delete equirectangularToCubemapShader;

		return environmentMap;
	}

	Texture2D CreateIrradianceMap(Texture2D environment, Window* window)
	{
		Cube* renderCube = CreateCube();

		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		Texture2D irradianceMap = CreateEmptyCubemap("irradiancecubemap", 32, 32);

		unsigned int captureFBO, captureRBO;
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		Shader* irradianceShader = CreateShader("irradianceConvolution", "Resources/Shaders/equirectangularToCubemapVS.glsl", "Resources/Shaders/irradianceConvolutionFS.glsl");
		irradianceShader->Use();
		irradianceShader->SetUniform("environmentMap", 0);
		irradianceShader->SetUniform("projection", captureProjection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, environment.id);

		glViewport(0, 0, 32, 32); // Set viewport to dimensions of the capture (32*32)
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		for (unsigned int i = 0; i < 6; i++)
		{
			irradianceShader->SetUniform("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap.id, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindVertexArray(renderCube->vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Clean up
		glViewport(0, 0, window->width, window->height); // Set viewport back to screen dimensions
		delete renderCube;
		delete irradianceShader;

		return irradianceMap;
	}

	Texture2D CreateSpecularMap(Texture2D environment, Window* window)
	{
		return Texture2D();
	}
}