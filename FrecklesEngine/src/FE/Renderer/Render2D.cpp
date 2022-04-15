#include "FE/Core/FEpch.hpp"
#include "FE/Renderer/Render2D.hpp"

#include "FE/Renderer/VertexArray.hpp"
#include "FE/Renderer/Buffers.hpp"
#include "FE/Renderer/RenderCommand.hpp"
#include "FE/Renderer/Shader.hpp"
#include "FE/Renderer/Texture.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <array>

namespace FE
{
	namespace RENDERER
	{
		struct Render2DVertexData
		{
			glm::vec3 Position; // 3*float = 12
			glm::vec4 Color;	// 4 * float = 16
			glm::vec2 Scale;	// 2 * float = 8
			glm::vec2 TexCoord; // 2 * float = 8
		};

		struct Render2DData
		{
			constexpr static uint32_t MaxQuads = 10000;
			constexpr static uint32_t MaxVertices = MaxQuads * 4;			
			constexpr static uint32_t MaxIndices = MaxQuads * 6;

			glm::vec4 QuadVertexPositions[4];
			glm::vec2 QuadTextureCoordinates[4];
			Ref<VertexArray> QuadVAO;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<Texture2D> WhiteTexture;

			uint32_t RenderQuadIndexOffset = 0;
			uint32_t RenderQuadVertexOffset=0;
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
				{"a_Scale", BufferElementType::Float2},
				{"a_TexCoord", BufferElementType::Float2} });

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
			//shader
			s_Data2D.TextureShader = Shader::Create("assets/shaders/CameraVertexColorShaderTexture.shader");
		}
		void Render2D::Shutdown()
		{

		}

		void Render2D::RenderQuad(const glm::vec3& position, const glm::vec2& scale, glm::vec4 Color)
		{
			if (s_Data2D.RenderQuadIndexOffset >= Render2DData::MaxIndices)
				EndCurentRenderBatch();
			
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3{ 0.0f,0.0f,1.0f }) *
				glm::scale(glm::mat4(1.0f), { scale.x,scale.y,1.0f });

			for (auto i = 0; i < 4; i++)
			{				
				s_Data2D.RenderQuadVertexData[s_Data2D.RenderQuadVertexOffset + i].Position = transform * s_Data2D.QuadVertexPositions[i];
				s_Data2D.RenderQuadVertexData[s_Data2D.RenderQuadVertexOffset + i].Scale = scale;
				s_Data2D.RenderQuadVertexData[s_Data2D.RenderQuadVertexOffset + i].TexCoord = s_Data2D.QuadTextureCoordinates[i];
				s_Data2D.RenderQuadVertexData[s_Data2D.RenderQuadVertexOffset + i].Color = Color;
			}
			s_Data2D.RenderQuadVertexOffset += 4;
			s_Data2D.RenderQuadIndexOffset += 6;
		}

		void Render2D::BeginScene(const glm::mat4& viewProjection)
		{			
			s_Data2D.TextureShader->Bind();
			s_Data2D.TextureShader->SetUniform("u_ViewProjection", viewProjection);
			NewBatch();
		}

		void Render2D::EndScene()
		{
			EndBatch();
		}

		void Render2D::EndCurentRenderBatch()
		{
			EndBatch();
			NewBatch();
		}

		void Render2D::NewBatch()
		{
			s_Data2D.RenderQuadIndexOffset = 0;
			s_Data2D.RenderQuadVertexOffset = 0;
		}

		void Render2D::EndBatch()
		{
			s_Data2D.QuadVertexBuffer->SetData(s_Data2D.RenderQuadVertexData.data(), sizeof(Render2DVertexData) * s_Data2D.RenderQuadVertexOffset);

			s_Data2D.WhiteTexture->Bind();
			s_Data2D.QuadVAO->Bind();
			Flush();
		}

		void Render2D::Flush()
		{
			if (s_Data2D.RenderQuadIndexOffset == 0)
				return;
			RenderCommand::DrawIndexed(s_Data2D.QuadVAO, s_Data2D.RenderQuadIndexOffset);
		}
	}
}