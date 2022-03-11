#pragma once

#include <Blind/Renderer/Camera.h>

namespace Blind
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera();

		void SetViewportSize(uint32_t width, uint32_t height);

		void SetOrthographic(float size, float nearClip, float farClip);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

	private:
		void RecalculateProjection();

		float m_OrthographicSize = 10.0f;
		float m_OrthgrahpicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};
}