#pragma once 
#include "FE/Core/Base.hpp"

#include "FE/Renderer/Buffers.hpp"
namespace FE
{
    namespace RENDERER
    {
        class VertexArray
        {
        public:
            VertexArray();
            ~VertexArray();

            void Bind() const;
            void Unbind() const;
            void SetVertexBuffer(Ref<VertexBuffer>& vertexBuffer);
            void SetIndexBuffer(Ref<IndexBuffer>& indexBuffer);
            
            uint32_t GetIndexCount() const;

            static Ref<VertexArray> Create();
        private:
            uint32_t RenderID;
            Ref<VertexBuffer> VB;
            Ref<IndexBuffer> IB;
        };
    }
}