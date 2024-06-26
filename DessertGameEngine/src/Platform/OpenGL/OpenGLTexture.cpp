#include "dgepch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Dessert {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		:m_Width(width), m_Height(height)
	{
		DGE_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		//1 for mipmap
		glTextureStorage2D(m_RendererId, 1, m_InternalFormat, m_Width, m_Height); // Comment OpenGL stocke la donnee

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			DGE_PROFILE_SCOPE("OpenGLTexture2D::OpenGLTexture2D(const std::string&) stbi_load");

			data = stbi_load(path.c_str(), &width, &height, &channels, 0); // Channels representent le nombre de donnees pour l'image (RGB = 3, RGBA = 4)
		}
		DGE_CORE_ASSERT(data, "Failed to load image");
		
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		DGE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		//1 for mipmap
		glTextureStorage2D(m_RendererId, 1, internalFormat, m_Width, m_Height); // Comment OpenGL stocke la donnee

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);



		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data); // Comment OpenGL exporte l'image

		stbi_image_free(data);
	
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		DGE_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		DGE_PROFILE_FUNCTION();

		uint32_t bytePerPixels = m_DataFormat == GL_RGBA ? 4 : 3;
		DGE_CORE_ASSERT(size == m_Width * m_Height * bytePerPixels, "Data must be entire texture");

		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data); // Comment OpenGL exporte l'image
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		DGE_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererId);
	}
}