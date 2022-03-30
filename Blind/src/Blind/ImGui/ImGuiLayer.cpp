#include <blindpch.h>
#include "ImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <Blind/Core/Application.h>
#include <Blind/Core/Core.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImGuizmo.h"

namespace Blind
{

	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer") {}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		BL_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/static/OpenSans/OpenSans-Bold.ttf", 18.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/static/OpenSans/OpenSans-Regular.ttf", 18.0f);

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		BL_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO io = ImGui::GetIO();
			e.handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::SetColorSceme(const ScemeType& type)
	{
		auto& colors= ImGui::GetStyle().Colors;
		ImGuiStyle* style = &ImGui::GetStyle();
		switch ((int)type)
		{
		case (int)ScemeType::Dark:
			style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4{ 0.07f, 0.075f, 0.073f, 1.0f };

			// Headers
			colors[ImGuiCol_Header] = ImVec4{ 0.18f, 0.195f, 0.19f, 1.0f };
			colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.25f, 0.285f, 0.31f, 1.0f };
			colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

			// Buttons
			colors[ImGuiCol_Button] = ImVec4{ 0.25f, 0.245f, 0.251f, 1.0f };
			colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.23f, 0.25f, 0.27f, 1.0f };
			colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

			// Frame BG
			colors[ImGuiCol_FrameBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
			colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
			colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

			colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.12f, 0.12f, 0.12f, 1.0f };
			colors[ImGuiCol_PopupBg] = ImVec4{ 0.09f, 0.09f, 0.09f, 1.0f };

			// Tabs
			colors[ImGuiCol_Tab] = ImVec4{ 0.08f, 0.06f, 0.97f, 1.0f };
			colors[ImGuiCol_TabHovered] = ImVec4{ 0.08f, 0.09f, 0.07f, 1.0f };
			colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
			colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

			// Title
			colors[ImGuiCol_TitleBg] = ImVec4{ 0.11f, 0.105f, 0.11f, 1.0f };
			colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
			break;
		case (int)ScemeType::White:
			style->Colors[ImGuiCol_Text] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };

			// Headers
			colors[ImGuiCol_Header] = ImVec4{ 0.972f, 0.975f, 0.973f, 1.0f };
			colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.75f, 0.67f, 0.63f, 1.0f };
			colors[ImGuiCol_HeaderActive] = ImVec4{ 0.65f, 0.6505f, 0.6151f, 1.0f };

			// Buttons
			colors[ImGuiCol_Button] = ImVec4{ 0.7645f, 0.7645f, 0.7641f, 1.0f };
			colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.793f, 0.790f, 0.789f, 1.0f };
			colors[ImGuiCol_ButtonActive] = ImVec4{ 0.79f, 0.793f, 0.789f, 1.0f };

			// Frame BG
			colors[ImGuiCol_FrameBg] = ImVec4{ 0.691f, 0.687f, 0.692f, 1.0f };
			colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.69f, 0.689f, 0.693f, 1.0f };
			colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.694f, 0.692f, 0.684f, 1.0f };

			colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.7f, 0.7f, 0.7f, 1.0f };
			colors[ImGuiCol_PopupBg] = ImVec4{ 0.9f, 0.9f, 0.9f, 1.0f };
			// Tabs
			colors[ImGuiCol_Tab] = ImVec4{ 0.689f, 0.688f, 0.69f, 1.0f };
			colors[ImGuiCol_TabHovered] = ImVec4{ 0.685f, 0.683f, 0.687f, 1.0f };
			colors[ImGuiCol_TabActive] = ImVec4{ 0.692f, 0.694f, 0.692f, 1.0f };
			colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.695f, 0.68805f, 0.69151f, 1.0f };
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.692f, 0.690205f, 0.6921f, 1.0f };

			// Title
			colors[ImGuiCol_TitleBg] = ImVec4{ 0.71f, 0.71f, 0.71f, 1.0f };
			colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.86915f, 0.8691505f, 0.869151f, 1.0f };
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.86915f, 0.8691505f, 0.861951f, 1.0f };
			break;
		}
	}

	void ImGuiLayer::Begin()
	{
		BL_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		BL_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.07f, 0.075f, 0.073f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.18f, 0.195f, 0.19f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.25f, 0.285f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.25f, 0.245f, 0.251f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.23f, 0.25f, 0.27f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.12f, 0.12f, 0.12f, 1.0f };
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.09f, 0.09f, 0.09f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.08f, 0.06f, 0.97f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.08f, 0.09f, 0.07f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.11f, 0.105f, 0.11f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}

}