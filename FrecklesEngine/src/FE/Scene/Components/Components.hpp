#pragma once
#include "FE/Core/Base.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace FE
{
	namespace COMPONENTS
	{
		struct NameComponent
		{
			NameComponent(const std::string& name) :Name{ name } {}
			std::string Name;
		};

		struct Transform
		{
			glm::vec3 Position{0.0f, 0.0f, 0.0f};
			glm::vec3 Rotation{0.0f, 0.0f, 0.0f};
			glm::vec3 Scale{1.0f, 1.0f, 1.0f};

			glm::mat4 GetTransform() const 
			{
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1,0,0 })
					* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0,1,0 })
					* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0,0,1 });

				return glm::translate(glm::mat4(1.0f), Position) * rotationMatrix *
					glm::scale(glm::mat4(1.0f), Scale);
			};
		};
	}
}