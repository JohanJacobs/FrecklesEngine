
#include "FE/Core/FEpch.hpp"
#include "FE/Core/Renderer/Buffers.hpp"

#include <glad/glad.h>
namespace FE
{
    namespace RENDERER
    {       
        namespace Utils
        {
            uint32_t BufferElementTypeSize(BufferElementType type)
            {
                switch (type)
                {
                    case BufferElementType::Float:  return sizeof(float);
                    case BufferElementType::Float2: return sizeof(float)*2;
                    case BufferElementType::Float3: return sizeof(float)*3;
                    case BufferElementType::Float4: return sizeof(float)*4;
                }
                LOG_CORE_ERROR("Invlaud RENDER::ElementType");
                return 0;
            }
        }
        /*
            BUFFER LAYOUT 
        */

        BufferLayout::BufferLayout(std::initializer_list<BufferElement> list)        
        {
            Elements = list;
            CalculateOffsets();
        }

        BufferLayout::BufferLayout(const BufferLayout& other)
        {
            Elements = other.Elements;
        }

        void BufferLayout::CalculateOffsets()
        {
            uint32_t offset = 0;
            for (auto& e : Elements)
            {
                e.Size =Utils::BufferElementTypeSize(e.Type);
                e.Offset = offset;
                offset += e.Size;
            }
        }
        /*
            VERTEX BUFFER
        */
        VertexBuffer::VertexBuffer(uint32_t size)
        :Size {size}
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glCreateBuffers(1,&RenderID);
        }

        VertexBuffer::~VertexBuffer()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glDeleteBuffers(1,&RenderID);
        }
        
        void VertexBuffer::Bind() const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glBindBuffer(GL_ARRAY_BUFFER, RenderID);
        }

        void VertexBuffer::Unbind() const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glBindBuffer(GL_ARRAY_BUFFER, 0);            
        }

        void VertexBuffer::SetData(void* data, uint32_t size)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            if (size != Size)
                LOG_CORE_ERROR("Invalid size VertexBuffer::SetData, expected {} but got {}",Size,size);
            glNamedBufferStorage(RenderID, size, data,GL_DYNAMIC_STORAGE_BIT);
        }

        uint32_t VertexBuffer::GetRenderID() const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            return RenderID;
        }

        Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            return CreateRef<VertexBuffer>(size);
        }

        /*
            INDEX BUFFER
        */

        IndexBuffer::IndexBuffer(const uint32_t* indexData, uint32_t count)
        :Count {count}
        {
            glCreateBuffers(1,&RenderID);
            glNamedBufferStorage(RenderID,sizeof(GL_UNSIGNED_INT)*count, indexData, GL_DYNAMIC_STORAGE_BIT);
        }

        IndexBuffer::~IndexBuffer()
        {
            glDeleteBuffers(1,&RenderID);
        }

        void IndexBuffer::Bind() const
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderID);
        }

        void IndexBuffer::Unbind() const
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        uint32_t IndexBuffer::GetRenderID() const
        {
            return RenderID;
        }

        uint32_t IndexBuffer::GetIndexCount() const
        {
            return Count;
        }

        Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indexData, uint32_t count)
        {
            return CreateRef<IndexBuffer>(indexData,count);
        }
    }
} 