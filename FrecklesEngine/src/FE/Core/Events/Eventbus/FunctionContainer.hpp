#pragma once
#include <functional>
#include <string>
namespace FE
{
    class BaseFunctionContainer
    {
    public:
        virtual ~BaseFunctionContainer() {}
        virtual const std::string& GetLabel() const = 0;
    };


    template<typename Args>
    class FunctionContainer :public BaseFunctionContainer
    {
        using FuncT = std::function<void(Args)>;
    public:
        FunctionContainer() {}
        FunctionContainer(const std::string& label, FuncT fn)
            :Label{ label }, StoredFunction{ std::forward<FuncT>(fn) }
        {}

        void Execute(Args args)
        {
            if (StoredFunction)
                StoredFunction(std::forward<Args>(args));
        }
        const std::string& GetLabel() const override { return Label; }
    private:
        FuncT StoredFunction;
        std::string Label;
    };
}