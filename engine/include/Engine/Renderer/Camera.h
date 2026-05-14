#pragma once

#include <glm/glm.hpp>

namespace Engine {

    class Camera {
    public:
        void SetPerspective(
            float fovRadians,
            float aspect,
            float nearPlane,
            float farPlane
        );

        void SetAspect(float aspect);

        void LookAt(
            const glm::vec3& eye,
            const glm::vec3& center,
            const glm::vec3& up
        );

        glm::mat4 GetView() const;
        glm::mat4 GetProjection() const;
        glm::mat4 GetViewProjection() const;

        glm::vec4 WorldToClip(
            const glm::vec3& worldPosition
        ) const;

        glm::vec3 WorldToNdc(
            const glm::vec3& worldPosition
        ) const;

        glm::vec3 GetPosition() const;
        glm::vec3 GetRight() const;
        glm::vec3 GetUp() const;

    private:
        void RecalculateProjection();

    private:
        glm::mat4 m_View{ 1.0f };
        glm::mat4 m_Projection{ 1.0f };

        glm::vec3 m_Position{ 0.0f };

        glm::vec3 m_Right{ 1.0f, 0.0f, 0.0f };
        glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };

        float m_FovRadians = 0.0f;
        float m_Aspect = 1.0f;
        float m_NearPlane = 0.1f;
        float m_FarPlane = 100.0f;
    };

}