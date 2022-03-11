#include <blindpch.h>
#include "OrthographicCameraController.h"

#include <Blind/Core/Input.h>
#include <Blind/Core/KeyCodes.h>
#include <Blind/Core/Log.h>

namespace Blind
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), m_Rotation(rotation)
	{
		CalculateView();
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		BL_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(Key::A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(Key::D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(Key::W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;

		else if (Input::IsKeyPressed(Key::S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::R))
				m_CameraRotation += m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		} 
		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel + 1.0f;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		BL_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		BL_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		BL_PROFILE_FUNCTION();

		OnResize((float)e.GetX(), (float)e.GetY());
		return false;
	}
	void OrthographicCameraController::OnResize(float width, float height)
	{
		BL_PROFILE_FUNCTION();

		m_AspectRatio = width / height;
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}
}