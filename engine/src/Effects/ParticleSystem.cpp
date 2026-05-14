#include "Engine/Effects/ParticleSystem.h"

#include <algorithm>

namespace Engine {

    void ParticleSystem::Emit(const Particle& particle) {
        m_Particles.push_back(particle);
    }

    void ParticleSystem::Update(float dt) {
        for (auto& particle : m_Particles) {
            particle.position += particle.velocity * dt;
            particle.lifetime -= dt;
        }

        m_Particles.erase(
            std::remove_if(
                m_Particles.begin(),
                m_Particles.end(),
                [](const Particle& p) {
                    return p.lifetime <= 0.0f;
                }
            ),
            m_Particles.end()
        );
    }

    const std::vector<Particle>& ParticleSystem::GetParticles() const {
        return m_Particles;
    }

}