#include "FE/Core/FEpch.hpp"
#include "FE/Renderer/VertexArray.hpp"

#include <glad/glad.h>
namespace FE
{
    namespace RENDERER
    {
        namespace Utils
        {
            GLenum BufferElementToOpenGLType(RENDERER::BufferElementType type )
            {
                switch (type)
                {
                    case RENDERER::BufferElementType::Float:    return GL_FLOAT;
                    case RENDERER::BufferElementType::Float2:   return GL_FLOAT;
                    case RENDERER::BufferElementType::Float3:   return GL_FLOAT;
                    case RENDERER::BufferElementType::Float4:   return GL_FLOAT;
                    case RENDERER::BufferElementType::Int:      return GL_FLOAT;
                }
                LOG_CORE_ERROR("Invalid RENDER::BufferElementType");
                return 0;
            }
        }

        VertexArray::VertexArray()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glCreateVertexArrays(1,&RenderID);
        }

		VertexArray::VertexArray(Ref<VertexBuffer>& vb, Ref<IndexBuffer>& ib)
		{
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glCreateVertexArrays(1, &RenderID);
            SetVertexBuffer(vb);
            SetIndexBuffer(ib);
		}
        
        VertexArray::~VertexArray()
        {
            //LOG_CORE_TRACE(LOG_FUNCTION_NAME); // spdlog crash

            glDeleteVertexArrays(1,&RenderID);
        }

        void VertexArray::Bind() const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glBindVertexArray(RenderID);
        }

        void VertexArray::Unbind() const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glBindVertexArray(0);
        }

        void VertexArray::SetVertexBuffer(Ref<VertexBuffer>& vertexBuffer)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            VB = vertexBuffer;
            auto& layout = VB->GetLayout();
            
            glVertexArrayVertexBuffer(RenderID, 0, VB->GetRenderID(), 0, layout.GetStride()); 
            
            int elementIndex = 0;
            int bindingIndex = 0; // TODO: make a member variable for multiple vertex buffers;

            for (auto& element : layout)
            {
                glVertexArrayAttribBinding(RenderID, elementIndex, bindingIndex);
                glVertexArrayAttribFormat(RenderID, elementIndex, element.Count ,Utils::BufferElementToOpenGLType(element.Type), element.Normalized, element.Offset);
                glEnableVertexArrayAttrib(RenderID, elementIndex);

                elementIndex += 1;
            }
        }

        void VertexArray::SetIndexBuffer(Ref<IndexBuffer>& indexBuffer)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            IB =indexBuffer;
            glVertexArrayElementBuffer(RenderID, IB->GetRenderID());
        }
        
        uint32_t VertexArray::GetIndexCount() const 
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            return IB->GetIndexCount();
        }

        Ref<VertexArray> VertexArray::Create()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            return CreateRef<VertexArray>();
        }

		Ref<VertexArray> VertexArray::Create(Ref<VertexBuffer>& vb, Ref<IndexBuffer>& ib)
		{
            return CreateRef<VertexArray>(vb,ib);
		}

	}
}