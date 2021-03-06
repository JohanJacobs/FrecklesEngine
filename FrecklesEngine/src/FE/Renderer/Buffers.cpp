
#include "FE/Core/FEpch.hpp"
#include "FE/Renderer/Buffers.hpp"

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
                    case BufferElementType::Int:    return sizeof(int) ;
                }
                LOG_CORE_ERROR("Invlaud RENDER::BufferElementType");
                return 0;
            }

            uint32_t BufferElementTypeItemCount(BufferElementType type)
            {
                switch (type)
                {
                    case BufferElementType::Float:  return 1;
                    case BufferElementType::Float2: return 2;
                    case BufferElementType::Float3: return 3;
                    case BufferElementType::Float4: return 4;
                    case BufferElementType::Int:    return 1;
                        
                }
                LOG_CORE_ERROR("Invlaud RENDER::BufferElementType");
                return 0;
            }            
        }
        /*
            BUFFER LAYOUT 
        */
        BufferLayout::BufferLayout(std::initializer_list<BufferElement> list)        
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            Elements = list;
            CalculateOffsets();
        }

        BufferLayout::BufferLayout(const BufferLayout& other)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            Elements = other.Elements;
            Stride = other.Stride;
        }

        void BufferLayout::CalculateOffsets()
        {
            uint32_t offset = 0;
            for (auto& e : Elements)
            {
                e.Size =Utils::BufferElementTypeSize(e.Type);
                e.Offset = offset;
                offset += e.Size;
                e.Count = Utils::BufferElementTypeItemCount(e.Type);
            }
            Stride = offset;
        }
        /*
            VERTEX BUFFER
        */
        VertexBuffer::VertexBuffer(uint32_t size)
        :Size {size}
        {
            glCreateBuffers(1,&RenderID);
            glNamedBufferStorage(RenderID, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
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
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
                        
            glNamedBufferSubData(RenderID,0, size, data);
        }

        uint32_t VertexBuffer::GetSize() const
        {
            return Size;
        }

        void VertexBuffer::SetLayout(BufferLayout& layout)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            Layout = layout;
        }
        
        const BufferLayout& VertexBuffer::GetLayout() const
        {
            return Layout;
        }
        
        uint32_t VertexBuffer::GetRenderID() const
        {   
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
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            return CreateRef<IndexBuffer>(indexData,count);
        }
    }
} 