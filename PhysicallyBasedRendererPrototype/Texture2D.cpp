#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION

// External includes
#include <glad/glad.h>
#include "stb_image.h"

namespace TSFYP
{
	Texture2D CreateTexture2D(const Texture2D::TextureType textureType, const std::string& filePath)
	{
		unsigned int id = 0;
		glGenTextures(1, &id);

		int width, height, noComponents;

		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &noComponents, 0);
		if (!data)
		{
			stbi_image_free(data);
			return Texture2D(0, Texture2D::TextureType::ALBEDO, 0, 0);
		}

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

		return Texture2D(id, textureType, (unsigned int)width, (unsigned int)height);
	}
}