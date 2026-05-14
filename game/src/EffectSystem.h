#pragma once

#include "Engine/Effects/ParticleSystem.h"

#include <glm/glm.hpp>

class EffectSystem {
public:
    void PlayHitEffect(const glm::vec3& position);
    void PlaySkillEffect(const glm::vec3& position);

    void Update(float dt);

    Engine::ParticleSystem& GetParticleSystem();

private:
    Engine::ParticleSystem m_ParticleSystem;
};