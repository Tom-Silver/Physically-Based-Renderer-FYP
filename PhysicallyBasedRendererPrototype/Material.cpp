#include "Material.h"

// External includes
#include <glad/glad.h>
#include <ImGui/imgui.h>
#include <iostream>

namespace TSFYP
{
	Material::~Material()
	{
		delete shader;
		shader = nullptr;

		for (int i = 0; i < textures.size(); i++)
		{
			glDeleteTextures(1, &textures[i].id);
		}
		textures.clear();

		for (int i = 0; i < uniforms.size(); i++)
		{
			delete uniforms[i];
			uniforms[i] = nullptr;
		}
		uniforms.clear();
	}

	void Material::CreateGui()
	{
		if (ImGui::Begin("Material Info"))
		{
			std::string text = "Name: " + name;
			ImGui::Text(text.c_str());

			shader->CreateGui();

			ImGui::Separator();

			for (Texture2D& texture : textures)
			{
				text = "Name: " + texture.name;
				ImGui::Text(text.c_str());

				text = "Type: ";
				switch (texture.textureType)
				{
				case Texture2D::TextureType::ALBEDO:
					text += "Albedo";
					break;
				case Texture2D::TextureType::AO:
					text += "Ambient occlusion";
					break;
				case Texture2D::TextureType::METALLIC:
					text += "Metallic";
					break;
				case Texture2D::TextureType::NORMAL:
					text += "Normal";
					break;
				case Texture2D::TextureType::ROUGHNESS:
					text += "Roughness";
					break;
				}
				ImGui::Text(text.c_str());

				text = "Width: " + std::to_string(texture.width);
				ImGui::Text(text.c_str());

				text = "Height: " + std::to_string(texture.height);
				ImGui::Text(text.c_str());

				text = "Delete " + texture.name;
				if (ImGui::Button(text.c_str()))
				{
					textures.erase(std::remove_if(textures.begin(), textures.end(), [&texture](const Texture2D& toCheck) { return toCheck.id == texture.id; }), textures.end());
				}

				ImGui::Separator();
			}

			for (IUniform* uniform : uniforms)
			{
				uniform->CreateGui();
			}

			ImGui::End();
		}
	}
}