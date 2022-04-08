#pragma once
#include "Core/Base.hpp"
#include "Core/Application/Application.hpp"

#include "Core/Log/Log.hpp"

namespace FE
{
    Ref<CORE::Application> CreateApplication()
    {
        LOG_CORE_TRACE(LOG_FUNCTION_NAME);
        return CreateRef<CORE::Application>();
    }
}