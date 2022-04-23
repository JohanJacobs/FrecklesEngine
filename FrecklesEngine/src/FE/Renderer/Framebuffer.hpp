#pragma once

#include "FE/Core/Base.hpp"
#include "FE/Renderer/Texture.hpp"
namespace FE
{
	namespace RENDERER
	{
		class FrameBuffer
		{
		public:
			FrameBuffer(uint32_t width, uint32_t height);
			~FrameBuffer();

			void Bind();
			void Unbind();
			
			Ref<Texture2D>& GetColorAtachment() { return ColorAttachment; }
			static Ref<FrameBuffer>  Create(uint32_t width, uint32_t height);
		private:
			uint32_t RenderID;
			Ref<Texture2D> ColorAttachment;
			uint32_t DepthStencilAttachmentRenderID;
			uint32_t Width, Height;
		};
	}
}

