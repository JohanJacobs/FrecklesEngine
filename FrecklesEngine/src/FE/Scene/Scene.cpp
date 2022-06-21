#include "FE/Scene/Scene.hpp"
#include "FE/Scene/Entity.hpp"

namespace FE
{
	
	FE::Entity Scene::CreateEntity(const std::string& EntityTag /*= "Entity"*/)
	{
		Entity entity{ Registry.create(),this };
		entity.AddComponent<COMPONENTS::NameComponent>(EntityTag);
		return entity;
	}

}