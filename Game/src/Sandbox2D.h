#pragma once

#include <BlindEngine.h>

class Sandbox2D : public Blind::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Blind::Timestep ts) override;
	virtual void OnImGuiDraw() override;
	virtual void OnEvent(Blind::Event& e) override;
private:
	Blind::OrthographicCameraController m_CameraController;

	Blind::Ref<Blind::Shader> m_FlatColorShader;
	Blind::Ref<Blind::VertexArray> m_SquareVA;

	Blind::Ref<Blind::Texture2D> m_CheckerboardTexture;
	Blind::Ref<Blind::Texture2D> m_PlayerTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};