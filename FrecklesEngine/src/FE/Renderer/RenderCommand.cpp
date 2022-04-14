
#include "FE/Core/FEpch.hpp"
#include "FE/Renderer/RenderCommand.hpp"
#include "FE/Renderer/Render2D.h"

namespace FE
{
    namespace RENDERER
    {
        Ref<RenderCommand::RenderCommandData> RenderCommand::s_RenderCommandData = nullptr;

        void RenderCommand::Init(Ref<CORE::Window>& mainWindow)
        {
            s_RenderCommandData = CreateRef<RenderCommandData>();
            s_RenderCommandData->GraphicsContext = mainWindow->GetGraphicsContext();

            Renderer2D::Init();
        }

        void RenderCommand::Shutdown()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
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

	}
}