#include "FE/Core/FEpch.hpp"
#include "FE/Renderer/Texture.hpp"

#include "stb_image.h"
#include <glad/glad.h>

namespace FE
{
	namespace RENDERER
	{

		Texture2D::Texture2D(const std::string& filePath)
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			// load and generate the texture
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(1); // flip image
			unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels,0);
			
			uint32_t ImageFormat = GL_RGB;
			if (data)
			{
				LOG_CORE_INFO("Loaded Texture '{}'", filePath.c_str());
				switch (nrChannels)
				{
				case 3: 
				{
					ImageFormat = GL_RGB;
					InternalFormat = GL_RGBA8;
					break;
				}
				case 4:
				{
					ImageFormat = GL_RGBA;
					InternalFormat = GL_RGBA8;
					break;
				}

				LOG_CORE_ERROR("Invalid image Format, Channels: {}", nrChannels);
				}

				Width = width;
				Height = height;
				// texture 
				glCreateTextures(GL_TEXTURE_2D, 1, &RenderID);
				glTextureStorage2D(RenderID, 1, InternalFormat, width, height);

				// set the texture wrapping/filtering options (on the currently bound texture object)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glTextureSubImage2D(RenderID, 0, 0, 0, width, height, ImageFormat, GL_UNSIGNED_BYTE, data);
				//glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				LOG_ERROR("file not loaded");
			}
			stbi_image_free(data);
		}

		Texture2D::Texture2D(uint32_t width, uint32_t height)
		{
			Width = width;
			Height = height;
			
			InternalFormat = GL_RGBA8;
			// texture 
			glCreateTextures(GL_TEXTURE_2D, 1, &RenderID);			
			glTextureStorage2D(RenderID, 1, InternalFormat, width, height);			

			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);						
		}

		Texture2D::~Texture2D()
		{
			glDeleteTextures(1, &RenderID);
		}

		void Texture2D::Bind(uint32_t slot) const
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, RenderID);
		}

		Ref<FE::RENDERER::Texture2D> Texture2D::Create(const std::string& filePath)
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			return CreateRef<Texture2D>(filePath);
		}

		Ref<FE::RENDERER::Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);
			return CreateRef<Texture2D>(width, height);
		}

	}
}