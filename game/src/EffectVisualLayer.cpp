#include "EffectVisualLayer.h"

#include <algorithm>
#include <cmath>

void EffectVisualLayer::SpawnEffect(
    EffectType type,
    const glm::vec3& worldPosition
) {
    VisualEffect effect;
    effect.type = type;
    effect.worldPosition = worldPosition;

    effect.age = 0.0f;
    effect.lifetime = 0.35f;

    effect.startSize = 0.20f;
    effect.endSize = 0.85f;

    effect.startColor =
        GetStartColor(type);

    effect.endColor =
        GetEndColor(type);

    m_Effects.push_back(effect);
}

void EffectVisualLayer::Update(
    float dt
) {
    for (VisualEffect& effect : m_Effects) {
        effect.age += dt;
    }

    m_Effects.erase(
        std::remove_if(
            m_Effects.begin(),
            m_Effects.end(),
            [](const VisualEffect& effect) {
                return effect.age >= effect.lifetime;
            }
        ),
        m_Effects.end()
    );
}

void EffectVisualLayer::SubmitRenderItems(
    Engine::Scene& scene
) const {
    for (const VisualEffect& effect : m_Effects) {
        float t =
            effect.age / effect.lifetime;

        t =
            std::clamp(
                t,
                0.0f,
                1.0f
            );

        float size =
            effect.startSize +
            (effect.endSize - effect.startSize) * t;

        glm::vec4 color =
            glm::mix(
                effect.startColor,
                effect.endColor,
                t
            );

        /*
            淡出
        */
        color.a *=
            1.0f - t;

        Engine::SpriteRenderItem sprite;
        sprite.worldPosition =
            effect.worldPosition;

        sprite.size =
            glm::vec2{
                size,
                size
        };

        sprite.color =
            color;

        sprite.rotationRadians =
            t * 3.1415926f;

        sprite.spriteType =
            static_cast<float>(
                static_cast<int>(effect.type)
                );

        scene.SubmitSprite(sprite);
    }
}

glm::vec4 EffectVisualLayer::GetStartColor(
    EffectType type
) const {
    switch (type) {
    case EffectType::HealSpark:
        return glm::vec4{
            0.45f,
            1.0f,
            0.55f,
            1.0f
        };

    case EffectType::MagicSpark:
        return glm::vec4{
            0.45f,
            0.65f,
            1.0f,
            1.0f
        };

    case EffectType::HitSpark:
    default:
        return glm::vec4{
            1.0f,
            0.85f,
            0.25f,
            1.0f
        };
    }
}

glm::vec4 EffectVisualLayer::GetEndColor(
    EffectType type
) const {
    switch (type) {
    case EffectType::HealSpark:
        return glm::vec4{
            0.15f,
            0.8f,
            0.25f,
            0.0f
        };

    case EffectType::MagicSpark:
        return glm::vec4{
            0.2f,
            0.3f,
            1.0f,
            0.0f
        };

    case EffectType::HitSpark:
    default:
        return glm::vec4{
            1.0f,
            0.25f,
            0.1f,
            0.0f
        };
    }
}