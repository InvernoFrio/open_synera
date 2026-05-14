#pragma once

#include <string>
#include <unordered_map>

namespace Engine {

    struct AnimationClip {
        std::string name;
        float duration = 0.0f;
    };

    class Animator {
    public:
        void AddClip(const AnimationClip& clip);
        void Play(const std::string& name);
        void Update(float dt);

        const std::string& GetCurrentAnimation() const;

    private:
        std::unordered_map<std::string, AnimationClip> m_Clips;
        std::string m_CurrentAnimation;
        float m_CurrentTime = 0.0f;
    };

}