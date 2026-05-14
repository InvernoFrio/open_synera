#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Engine {

    struct Particle {
        glm::vec3 position{ 0.0f };
        glm::vec3 velocity{ 0.0f };
        glm::vec4 color{ 1.0f };
        float lifetime = 1.0f;
    };

    class ParticleSystem {
    public:
        void Emit(const Particle& particle);
        void Update(float dt);

        const std::vector<Particle>& GetParticles() const;

    private:
        std::vector<Particle> m_Particles;
    };

}