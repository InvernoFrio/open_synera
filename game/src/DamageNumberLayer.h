#pragma once

#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/PixelRenderConfig.h"
#include "Engine/Scene/Scene.h"

#include <glm/glm.hpp>

#include <vector>

enum class DamageNumberType {
    Physical = 0,
    Magic,
    Heal
};

struct DamageNumber {
    int value = 0;

    DamageNumberType type =
        DamageNumberType::Physical;

    glm::vec3 worldPosition{ 0.0f };

    float age = 0.0f;
    float lifetime = 0.85f;

    glm::vec2 screenOffset{ 0.0f };
};

class DamageNumberLayer {
public:
    void ShowDamage(
        int value,
        const glm::vec3& worldPosition,
        DamageNumberType type
    );

    void ShowHeal(
        int value,
        const glm::vec3& worldPosition
    );

    void Update(float dt);

    void SubmitRenderItems(
        Engine::Scene& scene,
        const Engine::Camera& camera,
        const Engine::PixelRenderConfig& pixelConfig,
        uint32_t framebufferWidth,
        uint32_t framebufferHeight
    ) const;

private:
    glm::vec4 GetColor(
        DamageNumberType type
    ) const;

    bool WorldToScreen(
        const glm::vec3& worldPosition,
        const Engine::Camera& camera,
        const Engine::PixelRenderConfig& pixelConfig,
        uint32_t framebufferWidth,
        uint32_t framebufferHeight,
        glm::vec2& outScreen
    ) const;

private:
    std::vector<DamageNumber> m_Numbers;
};