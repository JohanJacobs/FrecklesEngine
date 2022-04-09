
#include "FE/Core/FEpch.hpp"
#include "FE/Core/Renderer/RenderCommand.hpp"
namespace FE
{
    namespace RENDERER
    {
        Ref<RenderCommand::RenderCommandData> RenderCommand::s_RenderCommandData = nullptr;

        void RenderCommand::Init(Ref<CORE::Window>& mainWindow)
        {
            s_RenderCommandData = CreateRef<RenderCommandData>();
            s_RenderCommandData->GraphicsContext = mainWindow->GetGraphicsContext();
        }

        void RenderCommand::Shutdown()
        {
            
        }

        void RenderCommand::ClearColor(float red,float green,float blue, float alpha)
        {
            s_RenderCommandData->GraphicsContext->ClearColor(red, green, blue, alpha);
        }
        void RenderCommand::Clear()
        {
            s_RenderCommandData->GraphicsContext->Clear();
        }
    }
}