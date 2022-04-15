#pragma once
#include "FE/Core/Base.hpp"

#include <initializer_list>
#include <vector>
#include <string>

namespace FE
{
    namespace RENDERER
    {
        enum class BufferElementType
        {
            Float,
            Float2,
            Float3,
            Float4,
            Int           
        };

        struct BufferElement
        {
            BufferElement() =default;
            BufferElement(const std::string& name, BufferElementType type, bool normalized=false)
                :Name{name}, Type{type}, Size{0}, Count{0}, Offset{0}, Normalized{normalized} {}
            std::string Name;
            BufferElementType Type;
            uint32_t Size;
            uint32_t Count;
            uint32_t Offset;
            bool Normalized;
        };
        
        class BufferLayout
        {
        public:
            BufferLayout()=default;
            BufferLayout (std::initializer_list<BufferElement> list);
            BufferLayout(const BufferLayout& other);
            
            uint32_t GetStride() const {return Stride;}

            std::vector<BufferElement>::iterator begin() {return Elements.begin();}
            std::vector<BufferElement>::iterator end() {return Elements.end();}
            std::vector<BufferElement>::const_iterator begin() const {return Elements.begin();}
            std::vector<BufferElement>::const_iterator end() const {return Elements.end();}
        private:
            void CalculateOffsets();
            std::vector<BufferElement> Elements;
            uint32_t Stride{0};
        };

        class VertexBuffer 
        {
        public:
        VertexBuffer(uint32_t size);
        ~VertexBuffer();
        
        void Bind() const;
        void Unbind() const;

        void SetData(void* data, uint32_t size);
        uint32_t GetSize() const;

        void SetLayout(BufferLayout& layout);
        const BufferLayout& GetLayout() const; 

        uint32_t GetRenderID() const;

        static Ref<VertexBuffer> Create(uint32_t size);
        private:
            uint32_t Size;
            uint32_t RenderID;
            BufferLayout Layout;
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