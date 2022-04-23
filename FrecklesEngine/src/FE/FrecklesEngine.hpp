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
#include "FE/Renderer/Buffers.hpp"
#include "FE/Renderer/RenderCommand.hpp"
#include "FE/Renderer/VertexArray.hpp"
#include "FE/Renderer/Shader.hpp"
#include "FE/Renderer/Texture.hpp"
#include "FE/Renderer/Framebuffer.hpp"



    static Ref<FE::CORE::Application> CreateApplication()
    {
        using namespace FE;
        using namespace CORE;
        Log::Init();
        LOG_CORE_TRACE(LOG_FUNCTION_NAME);
        
        return CreateRef<Application>();
    }
