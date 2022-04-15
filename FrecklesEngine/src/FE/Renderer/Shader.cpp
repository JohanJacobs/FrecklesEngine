#include "FE/Core/FEpch.hpp"
#include "FE/Renderer/Shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

namespace FE
{
    namespace RENDERER
    {
        namespace Utils
        {
            GLenum ShaderTypeToOpenGLType(ShaderType type)
            {            
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
                switch (type)
                {
                    case ShaderType::VertexShader: return "Vertex";
                    case ShaderType::FragmentShader: return "Fragment";
                }
                LOG_CORE_ERROR("ShaderTypeToOpenGLType: Invalid shader type! ");
                return {};
            }

            ShaderType StringToShaderType(const std::string& shaderTypeString)
            {
                if (shaderTypeString == "Vertex")
                    return ShaderType::VertexShader;
                else if (shaderTypeString == "Fragment")
                    return ShaderType::FragmentShader;
                
                LOG_CORE_ERROR("StringToShaderType received unknown Shader type string '{0}'",shaderTypeString);
                return ShaderType::None;
            }

            std::unordered_map<ShaderType, std::string> LoadShaderSourceFromFile(const std::string& shaderPath)
            {
                LOG_CORE_TRACE(LOG_FUNCTION_NAME);
                
                std::unordered_map<ShaderType, std::string> shaderSources;

                // open file and move to end.
                std::ifstream file(shaderPath);
                if (!file.is_open())
                {
                    //LOG_CORE_INFO("Working directionry {}", std::filesystem::current_path().string());
                    LOG_CORE_ERROR("Failed to open file '{}'", shaderPath);
                    return shaderSources;
                }

                // read into buffer 
                std::stringstream buffer; 
                buffer << file.rdbuf();
                std::string fileContent = buffer.str();
                file.close();

                // setup token information
                std::string token = R"(#type)";
                auto token_length = token.length();   

                auto idx = fileContent.find(token,0);
                while (idx != std::string::npos)
                {   
                    // shader type
                    auto idx_eol = fileContent.find('\n',idx);
                    std::string shader_type = fileContent.substr(idx + token_length+1, idx_eol-token_length-idx-1);
                    ShaderType shader = StringToShaderType(shader_type);

                    // shader source code 
                    auto idx2 = fileContent.find(token,idx_eol);
                    idx = idx2;

                    if (idx2 ==std::string::npos)
                        idx2 = fileContent.length() - idx;

                    std::string shaderSrc = fileContent.substr(idx_eol+1,idx2-idx_eol-1);

                    shaderSources[shader] = shaderSrc;
                }
                return shaderSources;               
            }
        }

        Shader::Shader(const std::string& vertSrc, const std::string& fragSrc)
        {
            std::unordered_map<ShaderType, std::string> shaderSrc{
                {ShaderType::VertexShader,vertSrc},
                {ShaderType::FragmentShader,fragSrc}
            };

            auto compliedShaders = CompileShaders(shaderSrc);
            RenderID = LinkShaders(compliedShaders);
        }

        Shader::Shader(const std::string& shaderPath)
        {
            auto shaderSrc = Utils::LoadShaderSourceFromFile(shaderPath);
            auto compliedShaders = CompileShaders(shaderSrc);
            RenderID = LinkShaders(compliedShaders);
        }

        Shader::~Shader()
        {
        }

        void Shader::Bind() const
        {
            glUseProgram(RenderID);
        }

        void Shader::Unbind()const
        {
            glUseProgram(0);
        }

		void Shader::SetUniform(const std::string& uniformName, const glm::mat4& matrix) const
		{
            auto location = glGetUniformLocation(RenderID, uniformName.c_str());
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
		}

		void Shader::SetUniform(const std::string& uniformName, int* intArray, uint32_t count) const
		{
			auto location = glGetUniformLocation(RenderID, uniformName.c_str());
			glUniform1iv(location, count, intArray);
		}

		Ref<Shader> Shader::Create(const std::string& vertSrc, const std::string& fragSrc)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            return CreateRef<Shader>(vertSrc,fragSrc);
        }

        Ref<Shader> Shader::Create(const std::string& shaderPath)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            return CreateRef<Shader>(shaderPath);
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
