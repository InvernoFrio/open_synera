#include "Engine/Math/CameraRay.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

    Ray ScreenPointToRay(
        const Camera& camera,
        double mouseX,
        double mouseY,
        uint32_t screenWidth,
        uint32_t screenHeight
    ) {
        if (screenWidth == 0 || screenHeight == 0) {
            Ray ray;
            return ray;
        }

        float x =
            static_cast<float>(
                (2.0 * mouseX) / static_cast<double>(screenWidth) - 1.0
                );

        float y =
            static_cast<float>(
                (2.0 * mouseY) / static_cast<double>(screenHeight) - 1.0
                );

        glm::vec4 nearPointNdc{
            x,
            y,
            0.0f,
            1.0f
        };

        glm::vec4 farPointNdc{
            x,
            y,
            1.0f,
            1.0f
        };

        glm::mat4 inverseViewProjection =
            glm::inverse(camera.GetViewProjection());

        glm::vec4 nearWorld =
            inverseViewProjection * nearPointNdc;

        glm::vec4 farWorld =
            inverseViewProjection * farPointNdc;

        nearWorld /= nearWorld.w;
        farWorld /= farWorld.w;

        Ray ray;
        ray.origin = glm::vec3(nearWorld);
        ray.direction = glm::normalize(
            glm::vec3(farWorld - nearWorld)
        );

        return ray;
    }

}