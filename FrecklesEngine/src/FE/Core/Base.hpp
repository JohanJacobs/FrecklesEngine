#pragma once 

#include <memory>
#include <string>

#ifdef _MSC_VER
#define LOG_FUNCTION_NAME __FUNCSIG__
#else
#define LOG_FUNCTION_NAME __PRETTY_FUNCTION__
#endif

template <typename T>
using Ref =  std::shared_ptr<T>;

template <typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{

    return std::make_shared<T>(std::forward<Args>(args)...);
}

