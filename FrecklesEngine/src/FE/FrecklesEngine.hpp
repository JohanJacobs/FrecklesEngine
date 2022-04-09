#pragma once
#include "FE/Core/Base.hpp"
#include "FE/Core/Application/Application.hpp"

#include "FE/Core/Log/Log.hpp"

namespace FE
{
    Ref<CORE::Application> CreateApplication()
    {
        LOG_CORE_TRACE(LOG_FUNCTION_NAME);
        return CreateRef<CORE::Application>();
    }
}