#include "FE/Core/FEpch.hpp"
#include "FE/Core/Renderer/Shader.hpp"

#include <glad/glad.h>

namespace FE
{
    namespace RENDERER
    {
        namespace Utils
        {
            GLenum ShaderTypeToOpenGLType(ShaderType type)
            {
                LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
                switch (type)
                {
                   case ShaderType::VertexShader: return GL_VERTEX_SHADER;
                   case ShaderType::FragmentShader: return GL_FRAGMENT_SHADER;
                }
                LOG_CORE_ERROR("ShaderTypeToOpenGLType: Invalid shader type! ");
                return GL_NONE;
            }

            std::string ShaderTypeToString(ShaderType type)
            {
                LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
                switch (type)
                {
                    case ShaderType::VertexShader: return "VERTEX SHADER";
                    case ShaderType::FragmentShader: return "FRAGMENT SHADER";
                }
                LOG_CORE_ERROR("ShaderTypeToOpenGLType: Invalid shader type! ");
                return GL_NONE;
            }
        }

        Shader::Shader(const std::string& vertSrc, const std::string& fragSrc)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            std::unordered_map<ShaderType, std::string> shaderSrc{
                {ShaderType::VertexShader,vertSrc},
                {ShaderType::FragmentShader,fragSrc}
            };

            auto compliedShaders = CompileShaders(shaderSrc);
            RenderID = LinkShaders(compliedShaders);
        }

        Shader::~Shader()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
        }

        void Shader::Bind() const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glUseProgram(RenderID);
        }

        void Shader::Unbind()const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            glUseProgram(0);
        }

        Ref<Shader> Shader::Create(const std::string& vertSrc, const std::string& fragSrc )
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            return CreateRef<Shader>(vertSrc,fragSrc);
        }

        std::unordered_map<ShaderType,uint32_t> Shader::CompileShaders(const std::unordered_map<ShaderType,std::string>& sources)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            std::unordered_map<ShaderType,uint32_t> compiledShaders;

            for (auto& [type,source] : sources)
            {
                unsigned int shader;
                shader = glCreateShader(Utils::ShaderTypeToOpenGLType(type));
                const char* src = source.c_str();
                glShaderSource(shader, 1, &src, NULL);
                glCompileShader(shader);
                {
                    int  success;
                    char infoLog[512];
                    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

                    if(!success)
                    {
                        glGetShaderInfoLog(shader, 512, NULL, infoLog);
                        LOG_CORE_ERROR("{} compilation failed: {}",Utils::ShaderTypeToString(type), infoLog );
                        glDeleteShader(shader);
                        break;
                        // TODO : this should be reworked. currently if one shader fails we still run the other shader
                    }
                }
                compiledShaders[type] = shader;
            }
            return compiledShaders;
        }

        uint32_t Shader::LinkShaders(const std::unordered_map<ShaderType, uint32_t>& compiledShaders)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            unsigned int shaderProgram; 
            shaderProgram = glCreateProgram();

            for (auto& [type,shader] : compiledShaders)
                glAttachShader(shaderProgram,shader);
            
            glLinkProgram(shaderProgram);
            {
                int success;
                char infoLog[512];
                glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
                if(!success) {
                    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                    LOG_CORE_ERROR("LinkShaders: Shader linking failed: {}", infoLog );
                }
            }
            glUseProgram(shaderProgram);
            
            for (auto& [type,shader] : compiledShaders)
                glDeleteShader(shader);
            
            glUseProgram(0);
            return shaderProgram;
        }
    }
}
