
#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const uint32_t s_MapWidht = 24;
static const char* s_MapTiles = 
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWDDDDWWWWWWWWWWWWWWW"
"WWWWDDDDDDDDDDDDDDWWWWWW"
"WWWDDDDDDDDDDDDDDDDWWWWW"
"WWWDDDDDDDDDDDDDDDDWWWWW"
"WWDDDDDDDDDDDDDDDDDDWWWW"
"WWDDDDDDDDDDDDDDDDDDDWWW"
"WDDDDDDDDDDDDDDDDDDDDWWW"
"WWDDDDDDDDDDDDDDDDDDDWWW"
"WWDDDDDDDDDDDDDDDDDDDWWW"
"WDDDDDDDDDDDDDDDDDDDDDWW"
"WDDDDDDDDDDDDDDDDDDDDWWW"
"WDDDDDDDDDDDDDDDDDDDWWWW"
"WWWWWWWDDDDDDWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
; 

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController((float)APPLICATION_WINDOW.GetWidth() / (float)APPLICATION_WINDOW.GetHeight())
{
}

void Sandbox2D::OnAttach()
{
	BL_PROFILE_FUNCTION();

	m_CheckerboardTexture = Blind::Texture2D::Create("assets/textures/checkerboard.png");
	m_SpriteSheet = Blind::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

	m_TextureStairs = Blind::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 11 }, { 128, 128 });
	m_TextureTree = Blind::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, {1, 2});
	
	m_MapWidth = s_MapWidht;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidht;
	
	s_TextureMap['D'] = Blind::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 });
	s_TextureMap['W'] = Blind::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 });

	m_Particle.ColorBegin = m_Particle_system_color_begin;
	m_Particle.ColorEnd = m_Particle_system_color_end;
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(5.0f); 
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

#if 0
	{
		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		BL_PROFILE_SCOPE("Renderer Draw");
		Blind::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Blind::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), { 0.2f, 0.8f, 0.4f, 1.0f });
		Blind::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Blind::Renderer2D::DrawQuad({ 0.f, 4.0f }, { 1.0f, 1.0f }, { 0.8f, 0.3, 0.3f, 1.0f });
		Blind::Renderer2D::DrawQuad({ 0.0, 1.0f }, { 1.0f, 3.0f }, m_SquareColor);
		Blind::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 15.0f, 15.0f }, m_CheckerboardTexture, 5.0f);


		Blind::Renderer2D::EndScene();

		Blind::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
				Blind::Renderer2D::DrawQuad({ x, y, }, {0.45f, 0.45f}, color);
			}
		}
		Blind::Renderer2D::EndScene();
	}
#endif

	if (Blind::Input::IsMouseButtonPressed(BL_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Blind::Input::GetMousePosition();
		auto width = APPLICATION_WINDOW.GetWidth();
		auto height = APPLICATION_WINDOW.GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 16; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	Blind::Renderer2D::BeginScene(m_CameraController.GetCamera());
	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	for (uint32_t y = 0; y < m_MapHeight; y++)
	{
		for (uint32_t x = 0; x < m_MapWidth; x++)
		{
			char tileType = s_MapTiles[x + y * m_MapWidth];
			Blind::Ref<Blind::SubTexture2D> texture;
			if (s_TextureMap.find(tileType) != s_TextureMap.end())
				texture = s_TextureMap[tileType];
			else
				texture = m_TextureStairs;

			Blind::Renderer2D::DrawQuad({x - m_MapWidth / 2.0f, y - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture);
		}
	}
	Blind::Renderer2D::EndScene();

	m_Particle.ColorBegin = m_Particle_system_color_begin;
	m_Particle.ColorEnd = m_Particle_system_color_end;
}

void Sandbox2D::OnImGuiDraw()
{
	BL_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	ImGui::ShowDemoWindow();

	auto stats = Blind::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Particle Color Begin", glm::value_ptr(m_Particle_system_color_begin));
	ImGui::ColorEdit4("Particle Color End", glm::value_ptr(m_Particle_system_color_end));
	ImGui::End();
}

void Sandbox2D::OnEvent(Blind::Event& e)
{
	m_CameraController.OnEvent(e);
}
