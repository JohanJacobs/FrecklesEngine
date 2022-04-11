#pragma once
#include "FE/Core/Base.hpp"
namespace FE
{
    namespace RENDERER
    {
        class VertexBuffer 
        {
        public:
        VertexBuffer(uint32_t size);
        ~VertexBuffer();
        
        void Bind() const;
        void Unbind() const;

        void SetData(void* data, uint32_t size);

        uint32_t GetRenderID() const;

        static Ref<VertexBuffer> Create(uint32_t size);
        private:
            uint32_t Size;
            uint32_t RenderID;
        };
    }
} 