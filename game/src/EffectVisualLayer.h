#pragma once

#include "Engine/Scene/Scene.h"

#include <glm/glm.hpp>

#include <vector>

enum class EffectType {
    HitSpark = 0,
    HealSpark,
    MagicSpark
};

struct VisualEffect {
    EffectType type = EffectType::HitSpark;

    glm::vec3 worldPosition{ 0.0f };

    float age = 0.0f;
    float lifetime = 0.35f;

    float startSize = 0.25f;
    float endSize = 0.75f;

    glm::vec4 startColor{ 1.0f };
    glm::vec4 endColor{ 1.0f };
};

class EffectVisualLayer {
public:
    void SpawnEffect(
        EffectType type,
        const glm::vec3& worldPosition
    );

    void Update(float dt);

    void SubmitRenderItems(
        Engine::Scene& scene
    ) const;

private:
    glm::vec4 GetStartColor(
        EffectType type
    ) const;

    glm::vec4 GetEndColor(
        EffectType type
    ) const;

private:
    std::vector<VisualEffect> m_Effects;
};