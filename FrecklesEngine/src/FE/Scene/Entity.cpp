#include "FE/Scene/Entity.hpp"
namespace FE
{
    
	Entity::Entity(const entt::entity entityID, Scene* scene) 
		:EnitityID(entityID), ActiveScene{ scene }
	{

	}

}