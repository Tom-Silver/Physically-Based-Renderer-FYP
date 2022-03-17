#pragma once

// External includes
#include <string>

namespace TSFYP
{
	struct Texture2D
	{
		enum class TextureType;

		Texture2D(const unsigned int _id, const TextureType _textureType, const unsigned int _width, const unsigned int _height)
			: id(_id)
			, textureType(_textureType)
			, width(_width)
			, height(_height)
		{}

		const enum class TextureType
		{
			ALBEDO,
			NORMAL,
			METALLIC,
			ROUGHNESS,
			AO
		} textureType;

		const unsigned int id;
		const unsigned int width;
		const unsigned int height;
	};

	Texture2D CreateTexture2D(const Texture2D::TextureType textureType, const std::string& filePath);
}