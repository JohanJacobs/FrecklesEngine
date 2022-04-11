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



        class IndexBuffer
        {
        public:
            IndexBuffer(const uint32_t* indexData, uint32_t count);
            ~IndexBuffer();
            void Bind() const;
            void Unbind() const;
            uint32_t GetRenderID() const;
            uint32_t GetIndexCount() const;
            static Ref<IndexBuffer> Create(const uint32_t* indexData, uint32_t count);
        private:
            uint32_t RenderID;
            uint32_t Count;
        };
    }
} 