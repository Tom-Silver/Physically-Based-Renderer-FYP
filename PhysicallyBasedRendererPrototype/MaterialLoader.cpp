#include "MaterialLoader.h"

// External includes
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

// Internal includes
#include "Material.h"
#include "ShaderCompiler.h"

using json = nlohmann::json;

namespace TSFYP
{
	Material* LoadMaterial(std::string name)
	{
		// Open file
		std::ifstream inFile;
		{
			std::string prefix = "Resources/Materials/";
			std::string fileName = "/material.json";
			inFile.open(prefix + name + fileName);

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

		Material* newMaterial = new Material();

		if (jFile.contains("name"))
		{
			newMaterial->name = jFile["name"];
		}

		if (jFile.contains("shader"))
		{
			json jShader = jFile["shader"];
			std::string fragment, shaderName, vertex;
			std::string prefix = "Resources/Shaders/";
			std::string fileType = ".glsl";
			
			if (jShader.contains("fragment"))
			{
				fragment = jShader["fragment"];
				fragment = prefix + fragment + fileType;
			}

			if (jShader.contains("name"))
			{
				shaderName = jShader["name"];
			}

			if (jShader.contains("vertex"))
			{
				vertex = jShader["vertex"];
				vertex = prefix + vertex + fileType;
			}

			newMaterial->shader = CreateShader(shaderName, vertex, fragment);
		}

		if (jFile.contains("textures"))
		{
			json jTextureArray = jFile["textures"];
			std::string prefix = "Resources/Materials/" + name + "/";

			for (int i = 0; i < jTextureArray.size(); i++)
			{
				json jTexture = jTextureArray[i];
				std::string textureName, filePath;
				Texture2D::TextureType textureType(Texture2D::TextureType::ALBEDO);


				if (jTexture.contains("fileName"))
				{
					std::string fileName = jTexture["fileName"];
					filePath = prefix + fileName;
				}

				if (jTexture.contains("name"))
				{
					textureName = jTexture["name"];
				}

				if (jTexture.contains("type"))
				{
					textureType = (Texture2D::TextureType)jTexture["type"];
				}

				Texture2D newTexture = CreateTexture2DUInt(textureName, textureType, filePath);
				newMaterial->textures.emplace_back(newTexture);
			}
		}

		if (jFile.contains("uniforms"))
		{
			json jUniformArray = jFile["uniforms"];

			for (int i = 0; i < jUniformArray.size(); i++)
			{
				json jUniform = jUniformArray[i]; // Implement
			}
		}

		return newMaterial;
	}
}