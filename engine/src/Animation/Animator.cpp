#include "Engine/Animation/Animator.h"

namespace Engine {

    void Animator::AddClip(const AnimationClip& clip) {
        m_Clips[clip.name] = clip;
    }

    void Animator::Play(const std::string& name) {
        if (m_Clips.find(name) == m_Clips.end()) {
            return;
        }

        m_CurrentAnimation = name;
        m_CurrentTime = 0.0f;
    }

    void Animator::Update(float dt) {
        if (m_CurrentAnimation.empty()) {
            return;
        }

        m_CurrentTime += dt;

        const auto& clip = m_Clips[m_CurrentAnimation];

        if (m_CurrentTime > clip.duration) {
            m_CurrentTime = 0.0f;
        }
    }

    const std::string& Animator::GetCurrentAnimation() const {
        return m_CurrentAnimation;
    }

}