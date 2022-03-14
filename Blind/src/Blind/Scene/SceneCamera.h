#pragma once

#include <Blind/Renderer/Camera.h>

namespace Blind
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
		SceneCamera();
		virtual ~SceneCamera();

		void SetViewportSize(uint32_t width, uint32_t height);

		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip;  RecalculateProjection(); }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip;  RecalculateProjection(); }

		void SetOrthographic(float size, float nearClip, float farClip);
		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return m_OrthgrahpicNear; }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicNearClip(float nearClip) { m_OrthgrahpicNear = nearClip;  RecalculateProjection(); }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip;  RecalculateProjection(); }
		
		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
		void RecalculateProjection();

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthgrahpicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};
}