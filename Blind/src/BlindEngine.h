#pragma once

/* Blind Engine Wrapper Header */

// This header includes every dependency that is used by Blind and is only
// included by client applications.

// Blind dependencies
#include <Blind/Core/Application.h>
#include <Blind/Core/Log.h>

#include <Blind/Core/Input.h>
#include <Blind/Core/KeyCodes.h>
#include <Blind/Core/MouseCodes.h>
#include <Blind/Core/Timestep.h>

#include <Blind/Core/Layer.h>
#include <Blind/ImGui/ImGuiLayer.h>

#include <Blind/Renderer/Shader.h>
#include <Blind/Renderer/Texture.h>
#include <Blind/Renderer/SubTexture2D.h>
#include <Blind/Renderer/VertexArray.h>
#include <Blind/Renderer/Buffer.h>
#include <Blind/Renderer/Renderer.h>
#include <Blind/Renderer/Renderer2D.h>
#include <Blind/Renderer/RenderCommand.h>
#include <Blind/Renderer/OrthographicCamera.h>

#include <Blind/Renderer/OrthographicCameraController.h>

#define APPLICATION Blind::Application::Get()
#define APPLICATION_WINDOW Blind::Application::Get().GetWindow()
