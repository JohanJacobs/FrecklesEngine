#pragma once
#include "FE/Core/Base.hpp"
#include "FE/core/Time/Timestep.hpp"

#include "FE/Renderer/Cameras/OrthographicCamera.h"

#include <glm/glm.hpp>

namespace FE
{
	namespace RENDERER
	{
		struct OrthographicProperties
		{
			OrthographicProperties() = default;

			glm::vec3 Position{0.0f, 0.0f, 10.0f};
			float TranslationSpeed{ 5.0f };
			float Rotation{0.0f};
			float RotationSpeed{ 1.0f };

			float AspectRatio{16.0f/9.0f};

			float Size{50.0f};
			float NearClip{0.01f};
			float FarClip{100.0f};

		};

		class OrthographicCameraController
		{
		public:
			OrthographicCameraController()=default;
			~OrthographicCameraController()=default;						

			void SetupCamera(const OrthographicProperties& props = OrthographicProperties());
			void SetAspectRatio(float aspectRatio);
			void OnUpdate(CORE::Timestep ts);
			
			float GetSize() const;
			void SetSize(float size);

			const glm::mat4& GetViewProjection() const;
		private:
			void CalculateMatrices();
		private:
			OrthographicCamera Camera;
			glm::vec3 Position;
			float TranslationSpeed;
			float Rotation,RotationSpeed;

			glm::mat4 ViewMatrix;
			glm::mat4 ViewProjectionMatrix;
		};

	}
}
