#pragma once

#include <BlindEngine.h>
#include "ParticleSystem.h"

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
	Blind::Ref<Blind::Texture2D> m_SpriteSheet;
	Blind::Ref<Blind::SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;

	glm::vec4 m_Particle_system_color_begin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	glm::vec4 m_Particle_system_color_end = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };;
};