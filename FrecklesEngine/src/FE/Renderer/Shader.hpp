#pragma once 
#include "FE/Core/Base.hpp"

#include <glm/glm.hpp>
#include <unordered_map>

namespace FE
{
    namespace RENDERER
    {
        enum class ShaderType
        {
            None,
            FragmentShader,
            VertexShader
        };

        class Shader
        {
        public:
            Shader(const std::string& vertSrc, const std::string& fragSrc);
            Shader(const std::string& shaderPath);
            ~Shader();
            Shader(const Shader& other) = default;

            void Bind() const;
            void Unbind()const;

            void SetUniform(const std::string& uniformName, const glm::mat4& matrix) const;
            void SetUniform(const std::string& uniformName, int* intArray, uint32_t count) const;

            static Ref<Shader> Create(const std::string& vertSrc, const std::string& fragSrc );
            static Ref<Shader> Create(const std::string& shaderPath);
        
        private:
            std::unordered_map<ShaderType,uint32_t> CompileShaders(const std::unordered_map<ShaderType,std::string>& sources);
            uint32_t LinkShaders(const std::unordered_map<ShaderType, uint32_t>& compiledShaders);
        private:            
            uint32_t RenderID;
        };
    }
}