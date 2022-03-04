#pragma once

/* Blind Engine Wrapper Header */

// This header includes every dependency that is used by Blind and is only
// included by client applications.

// Blind dependencies
#include <Blind/Application.h>
#include <Blind/EntryPoint.h>
#include <Blind/Log.h>

#include <Blind/Input.h>
#include <Blind/KeyCodes.h>
#include <Blind/MouseCodes.h>
#include <Blind/Core/Timestep.h>

#include <Blind/Layer.h>
#include <Blind/ImGui/ImGuiLayer.h>

#include <Blind/Renderer/Shader.h>
#include <Blind/Renderer/Texture.h>
#include <Blind/Renderer/VertexArray.h>
#include <Blind/Renderer/Buffer.h>
#include <Blind/Renderer/Renderer.h>
#include <Blind/Renderer/RenderCommand.h>
#include <Blind/Renderer/OrthographicCamera.h>
