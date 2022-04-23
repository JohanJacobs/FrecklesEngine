
#include "FE/Core/FEpch.hpp"
#include "FE/Renderer/RenderCommand.hpp"
#include "FE/Renderer/Render2D.hpp"



namespace FE
{
    namespace RENDERER
    {
        Ref<RenderCommand::RenderCommandData> RenderCommand::s_RenderCommandData = nullptr;

        void RenderCommand::Init(Ref<CORE::Window>& mainWindow)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            s_RenderCommandData = CreateRef<RenderCommandData>();
            s_RenderCommandData->GraphicsContext = mainWindow->GetGraphicsContext();

            Render2D::Init();
        }

        void RenderCommand::Shutdown()
        {
            Render2D::Shutdown();
        }

        void RenderCommand::DrawIndexed(Ref<VertexArray>& vao, uint32_t indices)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            if (indices == 0)
                indices = vao->GetIndexCount();
                
            s_RenderCommandData->GraphicsContext->DrawIndexed(indices);
        }
        void RenderCommand::ClearColor(const glm::vec4& color)
        {
            s_RenderCommandData->GraphicsContext->ClearColor(color);
        }
        void RenderCommand::Clear()
        {
            s_RenderCommandData->GraphicsContext->Clear();
        }

		glm::vec2 RenderCommand::GetWindowSize()
		{
            return s_RenderCommandData->GraphicsContext->GetWindowSize();
		}

		void RenderCommand::SetViewportSize(int x, int y, int width, int height)
		{
            s_RenderCommandData->GraphicsContext->SetViewportSize(x, y, width, height);
		}

		void RenderCommand::SetFlag(ContextFlags flag, bool state)
		{
            if (state)
                s_RenderCommandData->GraphicsContext->EnableFlag(flag);
            else
                s_RenderCommandData->GraphicsContext->DisableFlag(flag);
		}

	}
}