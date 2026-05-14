#include "Engine/Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

    void Camera::SetPerspective(
        float fovRadians,
        float aspect,
        float nearPlane,
        float farPlane
    ) {
        m_FovRadians = fovRadians;
        m_Aspect = aspect;
        m_NearPlane = nearPlane;
        m_FarPlane = farPlane;

        RecalculateProjection();
    }

    void Camera::SetAspect(float aspect) {
        if (aspect <= 0.0f) {
            return;
        }

        m_Aspect = aspect;

        RecalculateProjection();
    }

    void Camera::LookAt(
        const glm::vec3& eye,
        const glm::vec3& center,
        const glm::vec3& up
    ) {
        m_Position = eye;

        m_View = glm::lookAt(
            eye,
            center,
            up
        );
    }

    glm::mat4 Camera::GetView() const {
        return m_View;
    }

    glm::mat4 Camera::GetProjection() const {
        return m_Projection;
    }

    glm::mat4 Camera::GetViewProjection() const {
        return m_Projection * m_View;
    }

    glm::vec3 Camera::GetPosition() const {
        return m_Position;
    }

    void Camera::RecalculateProjection() {
        m_Projection = glm::perspective(
            m_FovRadians,
            m_Aspect,
            m_NearPlane,
            m_FarPlane
        );

        // Vulkan NDC 与 OpenGL 的 Y 方向不同
        m_Projection[1][1] *= -1.0f;
    }

}