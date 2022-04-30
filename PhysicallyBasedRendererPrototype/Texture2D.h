#pragma once

// External includes
#include <string>

namespace TSFYP
{
	struct Texture2D
	{
		enum class TextureType;

		Texture2D();
		Texture2D(const unsigned int _id, const std::string _name, const TextureType _textureType, const unsigned int _width, const unsigned int _height);
		~Texture2D();

		enum class TextureType
		{
			ALBEDO,
			NORMAL,
			METALLIC,
			ROUGHNESS,
			AO
		} textureType;

		unsigned int id;
		std::string name;
		unsigned int width;
		unsigned int height;
	};

	Texture2D CreateTexture2DUInt(const std::string name, const Texture2D::TextureType textureType, const std::string& filePath);
	Texture2D CreateTexture2DFloat(const std::string name, const Texture2D::TextureType textureType, const std::string& filePath);
	Texture2D CreateEmptyCubemap(const std::string name, const unsigned int width, const unsigned int height);
}