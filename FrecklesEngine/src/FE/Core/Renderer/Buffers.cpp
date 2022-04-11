
#include "FE/Core/FEpch.hpp"
#include "FE/Core/Renderer/Buffers.hpp"

#include <glad/glad.h>
namespace FE
{
    namespace RENDERER
    {       
        VertexBuffer::VertexBuffer(uint32_t size)
        {
            glCreateBuffers(1,&RenderID);
        }

        VertexBuffer::~VertexBuffer()
        {
            glDeleteBuffers(1,&RenderID);
        }
        
        void VertexBuffer::Bind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, RenderID);
        }

        void VertexBuffer::Unbind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);            
        }

        void VertexBuffer::SetData(void* data, uint32_t size)
        {
            glNamedBufferStorage(RenderID, size, data,GL_DYNAMIC_STORAGE_BIT);
        }

        uint32_t VertexBuffer::GetRenderID() const
        {
            return RenderID;
        }

        Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
        {
            return CreateRef<VertexBuffer>(size);
        }
    }
} 