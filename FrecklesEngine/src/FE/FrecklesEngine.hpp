#pragma once
// logger 
#include "FE/Core/Log/Log.hpp"
// CORE functions
#include "FE/Core/Base.hpp"
#include "FE/Core/Application/Application.hpp"
#include "FE/Core/Layer/Layer.hpp"
#include "FE/Core/Time/Timestep.hpp"
#include "FE/Core/Input/Input.hpp"
#include "FE/Core/Input/KeyCodes.hpp"
#include "FE/Core/Input/MouseCodes.hpp"

// Renderer 
#include "FE/Renderer/RenderCommand.hpp"
#include "Fe/Renderer/Render2D.hpp"
#include "FE/Renderer/VertexArray.hpp"
#include "FE/Renderer/Shader.hpp"
#include "FE/Renderer/Texture.hpp"
#include "FE/Renderer/Framebuffer.hpp"
#include "FE/Renderer/Buffers.hpp"

// Events
#include "FE/Core/Events/Events.hpp"
#include "FE/Core/Events/ApplicationEvents.hpp"
#include "FE/Core/Events/MouseEvents.hpp"


    static Ref<FE::CORE::Application> CreateApplication()
    {
        using namespace FE;
        using namespace CORE;
        Log::Init();
        LOG_CORE_TRACE(LOG_FUNCTION_NAME);
        
        return CreateRef<Application>();
    }
