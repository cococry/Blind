#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController((float)APPLICATION_WINDOW.GetWidth() / (float)APPLICATION_WINDOW.GetHeight())
{
}

void Sandbox2D::OnAttach()
{
	BL_PROFILE_FUNCTION();

	m_CheckerboardTexture = Blind::Texture2D::Create("assets/textures/checkerboard.png");
	m_PlayerTexture = Blind::Texture2D::Create("assets/textures/player.png");
}

void Sandbox2D::OnDetach()
{
	BL_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Blind::Timestep ts)
{
	BL_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	{
		BL_PROFILE_SCOPE("Renderer Prep");
		Blind::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Blind::RenderCommand::Clear();
	}

	{
		BL_PROFILE_SCOPE("Renderer Draw");
		Blind::Renderer2D::BeginScene(m_CameraController.GetCamera());

		//Blind::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Blind::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Blind::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		Blind::Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 5.0f);
		Blind::Renderer2D::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_PlayerTexture);

		Blind::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiDraw()
{
	BL_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Blind::Event& e)
{
	m_CameraController.OnEvent(e);
}
