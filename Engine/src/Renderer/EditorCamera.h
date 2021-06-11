#pragma once


#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include "HTypes.h"

namespace HEngine {

    class EditorCamera 
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void OnUpdate(float dt);
        void OnEvent(Event& e);

        inline float GetDistance() const { return m_Distance; }
        inline void SetDistance(float distance) { m_Distance = distance; }
        inline void SetFocus(Vec3 point) {
            m_FocalPoint = point; UpdateView();
        }

        inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

        const Mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const Mat4& GetProjectionMatrix() const { return m_Projection; }
        Mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

        Vec3 GetUpDirection() const;
        Vec3 GetRightDirection() const;
        Vec3 GetForwardDirection() const;
        const Vec3& GetPosition() const { return m_Position; }
        glm::quat GetOrientation() const;

        float GetPitch() const { return m_Pitch; }
        float GetYaw() const { return m_Yaw; }
    private:
        void UpdateProjection();
        void UpdateView();

        bool OnMouseScroll(MouseScrolledEvent& e);

        void MousePan(const Vec2& delta);
        void MouseRotate(const Vec2& delta);
        void MouseZoom(float delta);

        Vec3 CalculatePosition() const;

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;
    private:
        float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

        Mat4 m_ViewMatrix;
        Mat4 m_Projection;
        Vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        Vec2 m_InitialMousePosition = { 0.0f, 0.0f };
        Vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

        float m_Distance = 10.0f;
        float m_Pitch = 0.0f, m_Yaw = 0.0f;

        float m_ViewportWidth = 1280, m_ViewportHeight = 720;
    };

}