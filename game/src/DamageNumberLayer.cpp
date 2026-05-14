#include "DamageNumberLayer.h"

#include "Engine/Renderer/OverlayRenderItem.h"

#include <algorithm>
#include <cmath>
#include <string>

void DamageNumberLayer::ShowDamage(
    int value,
    const glm::vec3& worldPosition,
    DamageNumberType type
) {
    DamageNumber number;
    number.value = value;
    number.type = type;
    number.worldPosition = worldPosition;
    number.age = 0.0f;
    number.lifetime = 0.85f;

    m_Numbers.push_back(number);
}

void DamageNumberLayer::ShowHeal(
    int value,
    const glm::vec3& worldPosition
) {
    ShowDamage(
        value,
        worldPosition,
        DamageNumberType::Heal
    );
}

void DamageNumberLayer::Update(
    float dt
) {
    for (DamageNumber& number : m_Numbers) {
        number.age += dt;
    }

    m_Numbers.erase(
        std::remove_if(
            m_Numbers.begin(),
            m_Numbers.end(),
            [](const DamageNumber& number) {
                return number.age >= number.lifetime;
            }
        ),
        m_Numbers.end()
    );
}

void DamageNumberLayer::SubmitRenderItems(
    Engine::Scene& scene,
    const Engine::Camera& camera,
    const Engine::PixelRenderConfig& pixelConfig,
    uint32_t framebufferWidth,
    uint32_t framebufferHeight
) const {
    for (const DamageNumber& number : m_Numbers) {
        glm::vec2 screen;

        if (!WorldToScreen(
            number.worldPosition,
            camera,
            pixelConfig,
            framebufferWidth,
            framebufferHeight,
            screen
        )) {
            continue;
        }

        float t =
            number.age / number.lifetime;

        t =
            std::clamp(
                t,
                0.0f,
                1.0f
            );

        float rise =
            42.0f * t;

        float alpha =
            1.0f - t;

        glm::vec4 color =
            GetColor(number.type);

        color.a *= alpha;

        std::string text =
            std::to_string(number.value);

        float digitWidth =
            16.0f;

        float digitHeight =
            28.0f;

        float spacing =
            3.0f;

        float totalWidth =
            static_cast<float>(text.size()) * digitWidth +
            static_cast<float>(text.size() - 1) * spacing;

        glm::vec2 start =
            screen -
            glm::vec2{
                totalWidth * 0.5f,
                rise
        };

        /*
            轻微弹跳缩放。
        */
        float scale =
            1.0f + 0.25f * std::sin(t * 3.1415926f);

        for (size_t i = 0; i < text.size(); i++) {
            char ch =
                text[i];

            if (ch < '0' || ch > '9') {
                continue;
            }

            int digit =
                ch - '0';

            Engine::OverlayRenderItem item;
            item.position =
                start +
                glm::vec2{
                    static_cast<float>(i) * (digitWidth + spacing),
                    0.0f
            };

            item.size =
                glm::vec2{
                    digitWidth * scale,
                    digitHeight * scale
            };

            item.color =
                color;

            item.shapeType =
                Engine::OverlayShapeType::Digit;

            item.params =
                glm::vec4{
                    static_cast<float>(digit),
                    0.0f,
                    0.0f,
                    0.0f
            };

            scene.SubmitOverlay(item);
        }
    }
}

glm::vec4 DamageNumberLayer::GetColor(
    DamageNumberType type
) const {
    switch (type) {
    case DamageNumberType::Magic:
        return glm::vec4{
            0.55f,
            0.75f,
            1.0f,
            1.0f
        };

    case DamageNumberType::Heal:
        return glm::vec4{
            0.45f,
            1.0f,
            0.55f,
            1.0f
        };

    case DamageNumberType::Physical:
    default:
        return glm::vec4{
            1.0f,
            0.85f,
            0.25f,
            1.0f
        };
    }
}

bool DamageNumberLayer::WorldToScreen(
    const glm::vec3& worldPosition,
    const Engine::Camera& camera,
    const Engine::PixelRenderConfig& pixelConfig,
    uint32_t framebufferWidth,
    uint32_t framebufferHeight,
    glm::vec2& outScreen
) const {
    if (framebufferWidth == 0 ||
        framebufferHeight == 0) {
        return false;
    }

    Engine::PixelViewport viewport =
        Engine::CalculatePixelViewport(
            pixelConfig,
            framebufferWidth,
            framebufferHeight
        );

    if (!viewport.IsValid()) {
        return false;
    }

    glm::vec4 clip =
        camera.WorldToClip(
            worldPosition
        );

    if (clip.w <= 0.0f) {
        return false;
    }

    glm::vec3 ndc =
        glm::vec3(clip) / clip.w;

    if (ndc.x < -1.2f ||
        ndc.x > 1.2f ||
        ndc.y < -1.2f ||
        ndc.y > 1.2f) {
        return false;
    }

    float localX =
        (ndc.x * 0.5f + 0.5f) *
        static_cast<float>(viewport.width);

    /*
        注意：
        Camera projection 已经是 Vulkan Y 翻转版本。
        这里与 ScreenPointToRay 保持同一套坐标约定。
    */
    float localY =
        (ndc.y * 0.5f + 0.5f) *
        static_cast<float>(viewport.height);

    outScreen =
        glm::vec2{
            static_cast<float>(viewport.x) + localX,
            static_cast<float>(viewport.y) + localY
    };

    return true;
}