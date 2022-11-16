#include "FE/Core/FEpch.hpp"
#include "FE/Renderer/Render2D.hpp"

#include "FE/Renderer/VertexArray.hpp"
#include "FE/Renderer/Buffers.hpp"
#include "FE/Renderer/RenderCommand.hpp"
#include "FE/Renderer/Shader.hpp"


#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <array>

namespace FE
{	
	namespace RENDERER
	{
		struct Render2DVertexData
		{
			glm::vec3 Position; // 3 * float = 12 bytes
			glm::vec4 Color;	// 4 * float = 16 bytes
			glm::vec2 TexCoord; // 2 * float = 8 bytes
			int TexIndex;  		// 1 * uint32 = 4 bytes
			int TilingFactor; 	// 1 * uint32 = 4 bytes
			// unique ID for each object
		};

		struct Render2DData
		{
			constexpr static uint32_t MaxQuads = 10000;
			constexpr static uint32_t MaxVertices = MaxQuads * 4;			
			constexpr static uint32_t MaxIndices = MaxQuads * 6;
			constexpr static uint32_t MaxTextureSlots = 32;

			glm::vec4 QuadVertexPositions[4];
			glm::vec2 QuadTextureCoordinates[4];
			Ref<VertexArray> QuadVAO;
			Ref<VertexBuffer> QuadVertexBuffer;
			
			Ref<Texture2D> WhiteTexture;
			std::array<Ref<Texture2D>, MaxTextureSlots> RenderTextureSlots;

			uint32_t RenderQuadIndexOffset = 0;
			uint32_t RenderQuadVertexOffset=0;
			uint32_t RenderTextureSlotOffset = 1;
			std::array<Render2DVertexData, MaxVertices> RenderQuadVertexData;

			glm::mat4 ViewProjection;
			Ref<Shader> TextureShader;

		};

		static Render2DData s_Data2D;

		void Render2D::Init()
		{
			// create defaults
			s_Data2D.QuadVertexPositions[0] = { -1.0f,  1.0f, 0.0f, 1.0f }; // Top Left 
			s_Data2D.QuadVertexPositions[1] = { -1.0f, -1.0f, 0.0f, 1.0f }; // Bottom Left 
			s_Data2D.QuadVertexPositions[2] = {  1.0f, -1.0f, 0.0f, 1.0f }; // Bottom Right 
			s_Data2D.QuadVertexPositions[3] = {  1.0f,  1.0f, 0.0f, 1.0f }; // top Right

			s_Data2D.QuadTextureCoordinates[0] = { 0.0f, 1.0f }; // Top Left
			s_Data2D.QuadTextureCoordinates[1] = { 0.0f, 0.0f }; // Bottom Left
			s_Data2D.QuadTextureCoordinates[2] = { 1.0f, 0.0f }; // Bottom Right
			s_Data2D.QuadTextureCoordinates[3] = { 1.0f, 1.0f }; // Top Right
						
			s_Data2D.QuadVertexBuffer = VertexBuffer::Create(sizeof(Render2DVertexData) * Render2DData::MaxVertices);
			BufferLayout layout({
				{"a_Position", BufferElementType::Float3},
				{"a_Color", BufferElementType::Float4},
				{"a_TexCoord", BufferElementType::Float2}, 
				{"a_TexIndex", BufferElementType::Int},
				{"a_TilingFactor", BufferElementType::Int},
				});

			s_Data2D.QuadVertexBuffer->SetLayout(layout);

			uint32_t* indexData = new uint32_t[Render2DData::MaxIndices];
			{
				uint32_t default_indices[] = { 0, 1, 2, 2, 3, 0 };
				uint32_t offset = 0;
				for (uint32_t i = 0; i < Render2DData::MaxIndices; i+=6)
				{
					indexData[i + 0] = offset + default_indices[0];
					indexData[i + 1] = offset + default_indices[1];
					indexData[i + 2] = offset + default_indices[2];
					indexData[i + 3] = offset + default_indices[3];
					indexData[i + 4] = offset + default_indices[4];
					indexData[i + 5] = offset + default_indices[5];
					offset += 4;
				}
			}
			auto IB = IndexBuffer::Create(indexData, Render2DData::MaxIndices);
			s_Data2D.QuadVAO = VertexArray::Create(s_Data2D.QuadVertexBuffer, IB);
			delete[] indexData;

			// white Texture 
			s_Data2D.WhiteTexture = Texture2D::Create("assets/textures/white.png");
			s_Data2D.RenderTextureSlots[0] = s_Data2D.WhiteTexture;

			int32_t samplers[Render2DData::MaxTextureSlots];
			for (uint32_t i = 0; i < Render2DData::MaxTextureSlots; i++)
				samplers[i] = i;
			//shader
			s_Data2D.TextureShader = Shader::Create("assets/shaders/TexturesShader.shader");
			s_Data2D.TextureShader->Bind();
			s_Data2D.TextureShader->SetUniform("u_Textures", samplers, Render2DData::MaxTextureSlots);
		}
		void Render2D::Shutdown()
		{

		}

		void Render2D::RenderQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			CheckBatch();			
			auto transform = CalculateTransform(position, scale);		
			CreateVertices(transform, color);
		}

		void Render2D::RenderQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float rotationDegree)
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			CheckBatch();
			auto transform = CalculateTransform(position,scale,rotationDegree);
			CreateVertices(transform, color);
		}

		void Render2D::RenderTexture(Ref<Texture2D>& texture, const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			CheckBatch();
			auto transform = CalculateTransform(position, scale);
			int texIndex = GetTextureIndex(texture);
			CreateVertices(transform, color, texIndex);
		}

		void Render2D::RenderTexture(Ref<Texture2D>& texture, const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float rotationDegree)
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			CheckBatch();
			auto transform = CalculateTransform(position, scale, rotationDegree);
			int texIndex = GetTextureIndex(texture);
			CreateVertices(transform, color, texIndex);
		}

		void Render2D::BeginScene(const glm::mat4& viewProjection)
		{	
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			s_Data2D.TextureShader->Bind();
			s_Data2D.TextureShader->SetUniform("u_ViewProjection", viewProjection);
			NewBatch();
		}

		void Render2D::EndScene()
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			EndBatch();
		}

		void Render2D::CheckBatch()
		{
			if (s_Data2D.RenderQuadIndexOffset >= Render2DData::MaxIndices || s_Data2D.RenderTextureSlotOffset >= 32)
			{
				EndBatch();
				NewBatch();	
			}
		}

		void Render2D::NewBatch()
		{
			s_Data2D.RenderQuadIndexOffset = 0;
			s_Data2D.RenderQuadVertexOffset = 0;
			s_Data2D.RenderTextureSlotOffset = 1;
		}

		void Render2D::EndBatch()
		{
			if (s_Data2D.RenderQuadIndexOffset == 0)
				return;

			s_Data2D.QuadVertexBuffer->SetData(s_Data2D.RenderQuadVertexData.data(), sizeof(Render2DVertexData) * s_Data2D.RenderQuadVertexOffset);

			//bind textures 
			for (uint32_t i = 0; i < s_Data2D.RenderTextureSlotOffset; i++)
				s_Data2D.RenderTextureSlots[i]->Bind(static_cast<int>(i));
		
			s_Data2D.QuadVAO->Bind();

			Flush();
		}

		void Render2D::Flush()
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);
			
			RenderCommand::DrawIndexed(s_Data2D.QuadVAO, s_Data2D.RenderQuadIndexOffset);
		}

		glm::mat4 Render2D::CalculateTransform (const glm::vec3& position, const glm::vec2& scale)
		{
			return glm::translate(glm::mat4(1.0f), position) * 
				glm::scale(glm::mat4(1.0f), { scale.x,scale.y,1.0f });
		}

		glm::mat4 Render2D::CalculateTransform (const glm::vec3& position, const glm::vec2& scale, float rotationDegree)
		{
			return glm::translate(glm::mat4(1.0f), position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(rotationDegree), glm::vec3{ 0.0f,0.0f,1.0f }) *
				glm::scale(glm::mat4(1.0f), { scale.x,scale.y,1.0f });
		}

		int Render2D::GetTextureIndex(Ref<Texture2D>& texture)
		{
			int texIndex = -1; // default to white texture 
			for (uint32_t i = 0; i < s_Data2D.RenderTextureSlotOffset; i++)
			{
				if (texture == s_Data2D.RenderTextureSlots[i])
				{
					return static_cast<int>(i);
					break;
				}
			}
			
			s_Data2D.RenderTextureSlots[s_Data2D.RenderTextureSlotOffset] = texture;
			texIndex = s_Data2D.RenderTextureSlotOffset;
			s_Data2D.RenderTextureSlotOffset += 1;

			return texIndex;
		}

		void Render2D::CreateVertices(const glm::mat4& transform, const glm::vec4& color,int texIndex)
		{
			for (auto i = 0; i < 4; i++)
			{
				s_Data2D.RenderQuadVertexData[s_Data2D.RenderQuadVertexOffset + i].Position = transform * s_Data2D.QuadVertexPositions[i];
				s_Data2D.RenderQuadVertexData[s_Data2D.RenderQuadVertexOffset + i].TexCoord = s_Data2D.QuadTextureCoordinates[i];
				s_Data2D.RenderQuadVertexData[s_Data2D.RenderQuadVertexOffset + i].Color = color;
				s_Data2D.RenderQuadVertexData[s_Data2D.RenderQuadVertexOffset + i].TilingFactor = 1;
				s_Data2D.RenderQuadVertexData[s_Data2D.RenderQuadVertexOffset + i].TexIndex = texIndex;
			}

			s_Data2D.RenderQuadVertexOffset += 4;
			s_Data2D.RenderQuadIndexOffset += 6;
		}

	}
}