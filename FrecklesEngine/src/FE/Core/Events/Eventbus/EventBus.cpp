#include "FE/Core/FEpch.hpp"

#include "FE/Core/Events/Eventbus/EventBus.hpp"

std::unordered_map<std::type_index, std::vector<std::shared_ptr<FE::BaseFunctionContainer>>> FE::EventBus::Listeners;