#include "FE/Core/FEpch.hpp"
#include "FE/Renderer/Framebuffer.hpp"

#include <glad/glad.h>
namespace FE
{
	namespace RENDERER
	{
		namespace Utils
		{
			bool CheckFramebufferStatus(uint32_t fbo)
			{
				//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
				auto status = glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER);
				//glBindFramebuffer(GL_FRAMEBUFFER, 0);

				switch (status)
				{
				case GL_FRAMEBUFFER_UNDEFINED:LOG_ERROR("GL_FRAMEBUFFER_UNDEFINED"); return false;  break;
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); return false;  break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); return false;  break;
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"); return false; break;
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"); return false; break;
				case GL_FRAMEBUFFER_UNSUPPORTED:LOG_ERROR("GL_FRAMEBUFFER_UNSUPPORTED"); return false; break;
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"); return false;  break;
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"); return false; break;
				case GL_FRAMEBUFFER_COMPLETE: LOG_INFO("GL_FRAMEBUFFER_COMPLETE"); return true; break;
				};

				return false;
			}
		}

		FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
			:Width{width},Height{height}
		{			
			//create Framebuffer 
			glCreateFramebuffers(1, &RenderID);

			//create color attachment as a (texture) 
			ColorAttachment = Texture2D::Create(GLsizei(Width), GLsizei(Height));
			ColorAttachment->Unbind();
			glNamedFramebufferTexture(RenderID, GL_COLOR_ATTACHMENT0, ColorAttachment->GetRenderID(), 0);

			// depth and stencil buffer as render buffer     
			glCreateRenderbuffers(1, &DepthStencilAttachmentRenderID);
			glNamedRenderbufferStorage(DepthStencilAttachmentRenderID, GL_DEPTH24_STENCIL8, GLsizei(Width), GLsizei(Height));
			glNamedFramebufferRenderbuffer(RenderID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthStencilAttachmentRenderID);

			if (!Utils::CheckFramebufferStatus(RenderID))
			{
				LOG_CORE_ERROR("FrameBuffer is not complete!");
			}
		}

		void FrameBuffer::Resize(uint32_t width, uint32_t height)
		{
			// TODO: Check that we dont createa frame buffer that is out side of the max size;
			Width = width;
			Height = height;

			Invalidate();
		}
		
		void FrameBuffer::Invalidate()
		{
			//destroy the currrent frame buffer
			glDeleteFramebuffers(1, &RenderID);
			glDeleteRenderbuffers(1,&DepthStencilAttachmentRenderID);

			//create a new frame buffer
			glCreateFramebuffers(1,&RenderID);

			//color attachments
			ColorAttachment = Texture2D::Create(GLsizei(Width), GLsizei(Height));
			ColorAttachment->Unbind();
			glNamedFramebufferTexture(RenderID,GL_COLOR_ATTACHMENT0, ColorAttachment->GetRenderID(), 0);

			// depth and stencil buffer as render buffer
			glCreateRenderbuffers(1, &DepthStencilAttachmentRenderID);
			glNamedRenderbufferStorage(DepthStencilAttachmentRenderID, GL_DEPTH24_STENCIL8, GLsizei(Width),GLsizei(Height));
			glNamedFramebufferRenderbuffer(RenderID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthStencilAttachmentRenderID);

			if (!Utils::CheckFramebufferStatus(RenderID))
			{
				LOG_CORE_ERROR ("Framebuffer is not complete after resize!");
			}
		}

		FrameBuffer::~FrameBuffer()
		{
			glDeleteFramebuffers(1, &RenderID);
			glDeleteRenderbuffers(1, &DepthStencilAttachmentRenderID);
		}

		void FrameBuffer::Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, RenderID);
		}

		void FrameBuffer::Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
				
		Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height)
		{
			return CreateRef<FrameBuffer>(width, height);
		}

	}
}
