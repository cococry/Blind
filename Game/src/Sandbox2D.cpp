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
	
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Blind::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Blind::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Blind::RenderCommand::Clear();

	Blind::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
	Blind::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Blind::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });

	Blind::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiDraw()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Blind::Event& e)
{
	m_CameraController.OnEvent(e);
}
