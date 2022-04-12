#pragma once 
#include "FE/Core/Base.hpp"

#include <unordered_map>
namespace FE
{
    namespace RENDERER
    {
        enum class ShaderType
        {
            FragmentShader,
            VertexShader
        };

        class Shader
        {
        public:
            Shader(const std::string& vertSrc, const std::string& fragSrc);
            ~Shader();
            Shader(const Shader& other) = default;

            void Bind() const;
            void Unbind()const;

            static Ref<Shader> Create(const std::string& vertSrc, const std::string& fragSrc );
        
        private:
            std::unordered_map<ShaderType,uint32_t> CompileShaders(const std::unordered_map<ShaderType,std::string>& sources);
            uint32_t LinkShaders(const std::unordered_map<ShaderType, uint32_t>& compiledShaders);
        private:            
            uint32_t RenderID;
        };
    }
}