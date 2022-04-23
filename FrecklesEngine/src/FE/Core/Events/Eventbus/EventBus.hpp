#pragma once 
#include<string>
#include<unordered_map>
#include <typeindex>
#include <memory>

#include "FunctionContainer.hpp"
namespace FE
{
#define BIND_EVENT_FN(fn) [&](auto&&... args)->decltype(auto) { fn(std::forward<decltype(args)>(args)...); }
    class EventBus
    {
    public:
        /*
            USAGE AddListener<ParameterType>("UniqueLabel_for_this_Lambda",BINDFN(lambda));
            Eg. AddListener<std::string&>("UniqueLabel_for_this_Lambda",str);
            Eg. AddListener<ClassName&>("UniqueLabel_for_this_Lambda",object);

            The unique identifier (label) and "ParameterType" defines a combination that will be used when Adding a listener.

        */
        template<typename ParamT, typename FuncType>
        static void AddListener(const std::string& label, FuncType&& fn)
        {
            auto typeIndex = std::type_index(typeid(ParamT)); // Create an index for containers
            Listeners[typeIndex].push_back(std::make_shared<FunctionContainer<ParamT>>(label, fn)); // save a copy of the function (fn)
        }

        /*
            USAGE: RemoveListener<ParameterType>(stringValue);
            Eg. RemoveListener<ParameterType>("UniqueLabel_for_this_Lambda");

            The unique identifier (label) and "ParameterType" defines a combination that will be searched for when removing a listener.

        */
        template<typename ParamT>
        static void RemoveListener(const std::string& label)
        {
            auto typeIndex = std::type_index(typeid(ParamT));// Create an index for containers

            if (Listeners[typeIndex].size() == 0)
                return;

            //identify all stored functions with the matching parameter and label combination
            auto rm = std::remove_if(Listeners[typeIndex].begin(), Listeners[typeIndex].end(),
                [&](std::shared_ptr<BaseFunctionContainer>& obj1)
                {
                    return obj1->GetLabel() == label;
                });

            Listeners[typeIndex].erase(rm, Listeners[typeIndex].end());
        };

        /*
            USAGE:PushEvent<ParameterType>(param);

            All listeners that match the ArgumentType will receive the arguments
        */
        template<typename ParamT>
        static void PushEvent(ParamT event)
        {
            auto typeIndex = std::type_index(typeid(ParamT&));// Create an index for containers

            if (Listeners[typeIndex].size() == 0)
                return;

            // call all functions that match the Parameter types.
            for (auto& fn : Listeners[typeIndex])
            {
                std::static_pointer_cast<FunctionContainer<ParamT>>(fn)->Execute(event);
            }
        }

    private:
        static std::unordered_map<std::type_index, std::vector<std::shared_ptr<BaseFunctionContainer>>> Listeners;
    };
}
