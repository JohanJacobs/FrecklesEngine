#pragma once

#include "FE/Core/Base.hpp"
#include "FE/Renderer/Texture.hpp"


/*
	TODO: 
		- When creating / resizing a frame buffer make sure that it is not bigger than what is supported by the current graphics card
*/
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
			
			void Resize(uint32_t width, uint32_t height);

			Ref<Texture2D>& GetColorAtachment() { return ColorAttachment; }
			static Ref<FrameBuffer>  Create(uint32_t width, uint32_t height);
		private: 
			void Invalidate();
		private:
			uint32_t RenderID;
			Ref<Texture2D> ColorAttachment;
			uint32_t DepthStencilAttachmentRenderID;
			uint32_t Width, Height;
		};
	}
}

