#include <memory>

#define LOG_FUNCTION_NAME __PRETTY_FUNCTION__


template <typename T>
using Ref =  std::shared_ptr<T>;

template <typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#include "FE/Core/Log/Log.hpp"