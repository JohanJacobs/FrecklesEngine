#pragma once 
#include "FE/Core/Base.hpp"
#include "FE/Scene/Scene.hpp"
#include "FE/Scene/Components/Components.hpp"

namespace FE
{
    class Entity
    {
        public:
			Entity() = default;
            Entity(const Entity& other) = default;
			~Entity() = default;

            Entity(const entt::entity entityID, Scene* scene);           

            template<typename ComponentType>
            bool HasComponent() const
            {
                return ActiveScene->Registry.all_of<ComponentType>(EnitityID);
            }

            template<typename ComponentType, typename ...Args> 
            ComponentType& AddComponent(Args&&... args)
            {
                auto& component = ActiveScene->Registry.emplace<ComponentType>(EnitityID, std::forward<Args>(args)...);
                return component;
            }

            template<typename ComponentType>
            ComponentType& GetComponent()
            {
                if (!HasComponent<ComponentType>())
                {
                    LOG_CORE_ERROR("Entity does not have component!");
                    __debugbreak();
                }

                return ActiveScene->Registry.get<ComponentType>(EnitityID);
            }
            template<typename ComponentType>
            void RemoveComponent()
            {
                ActiveScene->Registry.remove<ComponentType>(EnitityID);
            }

        private:       
            entt::entity EnitityID;
            Scene* ActiveScene = nullptr;
                        
    };
}