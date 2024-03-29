#include "FE/Core/FEpch.hpp"
#include "FE/Core/Input/Input.hpp"

#include "FE/Renderer/Cameras/OrthographicCameraController.hpp"

namespace FE
{
	namespace RENDERER
	{
		void OrthographicCameraController::SetupCamera(const OrthographicProperties& props /*= OrthographicProperties()*/)
		{
			Camera.SetAspectRatio(props.AspectRatio);
			Camera.SetClipSpace(props.Size, props.NearClip, props.FarClip);
			Position = props.Position;
			TranslationSpeed = props.TranslationSpeed;

			Rotation = props.Rotation;
			RotationSpeed = props.RotationSpeed;
			CalculateMatrices();
		}

		void OrthographicCameraController::SetAspectRatio(float aspectRatio)
		{
			Camera.SetAspectRatio(aspectRatio);
			CalculateMatrices();
		}

		void OrthographicCameraController::OnUpdate(CORE::Timestep ts)
		{
			using namespace CORE;
			bool changed = false;
			if (Input::Keypressed(KeyCode::W)) // up
			{
				Position.y += TranslationSpeed * ts;
				changed = true;
			}
			else if (Input::Keypressed(KeyCode::S)) // down
			{
				Position.y -= TranslationSpeed * ts;
				changed = true;
			}

			if (Input::Keypressed(KeyCode::A)) // left
			{
				Position.x -= TranslationSpeed * ts;
				changed = true;
			}
			else if (Input::Keypressed(KeyCode::D)) // right
			{
				Position.x += TranslationSpeed * ts;
				changed = true;
			}

			if (Input::Keypressed(KeyCode::Q)) // rot left
			{
				Rotation -= RotationSpeed * ts;
				changed = true;
			}
			else if (Input::Keypressed(KeyCode::E)) //rot right
			{
				Rotation += RotationSpeed * ts;
				changed = true;
			}

			if (changed)
				CalculateMatrices();
		}

		float OrthographicCameraController::GetSize() const
		{
			return Camera.GetSize();
		}

		void OrthographicCameraController::SetSize(float size)
		{
			Camera.SetSize(size);
			CalculateMatrices();
		}

		const glm::mat4& OrthographicCameraController::GetViewProjection() const
		{
			return ViewProjectionMatrix;
		}

		void OrthographicCameraController::CalculateMatrices()
		{
			auto transform = glm::translate(glm::mat4(1.0f), Position) * 
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3{ 0.0f,0.0f,1.0f });

			ViewMatrix = glm::inverse(transform);
			ViewProjectionMatrix = Camera.GetProjection() * ViewMatrix;
		}

	}
}