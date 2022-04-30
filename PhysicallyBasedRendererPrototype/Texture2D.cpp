#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION

// External includes
#include <glad/glad.h>
#include "stb_image.h"

namespace TSFYP
{
	Texture2D::Texture2D()
		: id(0)
		, name("")
		, textureType(TextureType::ALBEDO)
		, width(0)
		, height(0)
	{}

	Texture2D::Texture2D(const unsigned int _id, const std::string _name, const TextureType _textureType, const unsigned int _width, const unsigned int _height)
		: id(_id)
		, name(_name)
		, textureType(_textureType)
		, width(_width)
		, height(_height)
	{}

	Texture2D::~Texture2D()
	{}

	Texture2D CreateTexture2DUInt(const std::string name, const Texture2D::TextureType textureType, const std::string& filePath)
	{
		int width, height, noComponents;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &noComponents, 0);
		if (!data)
		{
			stbi_image_free(data);
			return Texture2D(0, name, Texture2D::TextureType::ALBEDO, 0, 0);
		}

		// Data successfully loaded, so we can generate a texture
		unsigned int id = 0;
		glGenTextures(1, &id);

		// Set texture format based on loaded data
		GLenum format = GL_RED;
		if (noComponents == 2) { format = GL_RG; }
		else if (noComponents == 3) { format = GL_RGB; }
		else if (noComponents == 4) { format = GL_RGBA; }

		// Bind texture object and assign data
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set texture wrap and filtering parameters on bound texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		return Texture2D(id, name, textureType, (unsigned int)width, (unsigned int)height);
	}

	Texture2D CreateTexture2DFloat(const std::string name, const Texture2D::TextureType textureType, const std::string& filePath)
	{
		int width, height, noComponents;

		stbi_set_flip_vertically_on_load(true);
		float* data = stbi_loadf(filePath.c_str(), &width, &height, &noComponents, 0);
		if (!data)
		{
			stbi_image_free(data);
			return Texture2D(0, name, Texture2D::TextureType::ALBEDO, 0, 0);
		}

		// Data successfully loaded, so we can generate a texture
		unsigned int id = 0;
		glGenTextures(1, &id);

		// Set texture format based on loaded data
		GLenum format = GL_RED;
		GLenum internalFormat = GL_R16F;
		if (noComponents == 2) { format = GL_RG; internalFormat = GL_RG16F; }
		else if (noComponents == 3) { format = GL_RGB; internalFormat = GL_RGB16F; }
		else if (noComponents == 4) { format = GL_RGBA; internalFormat = GL_RGBA16F; }

		// Bind texture object and assign data
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set texture wrap and filtering parameters on bound texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		return Texture2D(id, name, textureType, (unsigned int)width, (unsigned int)height);
	}

	Texture2D CreateEmptyCubemap(const std::string name, const unsigned int width, const unsigned int height)
	{
		unsigned int id = 0;
		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		GLenum internalFormat = GL_RGB16F;
		GLenum format = GL_RGB;
		GLenum type = GL_FLOAT;

		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, nullptr);
		}

		// Set texture wrap and filtering parameters on bound texture
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		Texture2D::TextureType textureType = Texture2D::TextureType::ALBEDO;
		return Texture2D(id, name, textureType, (unsigned int)width, (unsigned int)height);
	}
}