#pragma once 
#include "FE/Core/Base.hpp"
#include "entt/entt.hpp"

namespace FE
{
	class Entity;

	class Scene
	{
	public:
		Scene() {};
		~Scene() {};

		Entity CreateEntity(const std::string& EntityTag = "Entity");
		
	private:
		friend class Entity;
		entt::registry Registry;
	};
}