#include "EffectSystem.h"

void EffectSystem::PlayHitEffect(const glm::vec3& position) {
    for (int i = 0; i < 16; i++) {
        Engine::Particle particle;
        particle.position = position;
        particle.velocity = {
            static_cast<float>((i % 4) - 2),
            2.0f,
            static_cast<float>((i / 4) - 2)
        };
        particle.color = { 1.0f, 0.3f, 0.1f, 1.0f };
        particle.lifetime = 0.5f;

        m_ParticleSystem.Emit(particle);
    }
}

void EffectSystem::PlaySkillEffect(const glm::vec3& position) {
    for (int i = 0; i < 64; i++) {
        Engine::Particle particle;
        particle.position = position;
        particle.velocity = {
            static_cast<float>((i % 8) - 4) * 0.4f,
            1.5f,
            static_cast<float>((i / 8) - 4) * 0.4f
        };
        particle.color = { 0.2f, 0.6f, 1.0f, 1.0f };
        particle.lifetime = 1.0f;

        m_ParticleSystem.Emit(particle);
    }
}

void EffectSystem::Update(float dt) {
    m_ParticleSystem.Update(dt);
}

Engine::ParticleSystem& EffectSystem::GetParticleSystem() {
    return m_ParticleSystem;
}