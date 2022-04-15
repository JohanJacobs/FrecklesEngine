#pragma once
#include "FE/Core/Base.hpp"

#include "FE/Renderer/Context.hpp"
#include "FE/Renderer/Texture.hpp"

#include <glm/glm.hpp>

namespace FE
{
	namespace RENDERER
	{
		class Render2D
		{
		public:
			Render2D() = delete;

			static void RenderQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
			static void RenderQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float rotationDegree);

			static void RenderTexture(Ref<Texture2D>& texture, const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
			static void RenderTexture(Ref<Texture2D>& texture, const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float rotationDegree);

			static void BeginScene(const glm::mat4& viewProjection);
			static void EndScene();

			static void Init();
			static void Shutdown();
			
		private:
			static void CheckBatch();
			static void NewBatch();
			static void EndBatch();
			static void Flush();

			static glm::mat4 CalculateTransform (const glm::vec3& position, const glm::vec2& scale);
			static glm::mat4 CalculateTransform (const glm::vec3& position, const glm::vec2& scale, float rotationDegree);

			static int GetTextureIndex(Ref<Texture2D>& texture);

			static void CreateVertices(const glm::mat4& transform, const glm::vec4& color, int texIndex=0);
		};
	}
}
