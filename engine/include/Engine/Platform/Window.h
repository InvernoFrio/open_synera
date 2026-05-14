#pragma once

#include <cstdint>
#include <string>

struct GLFWwindow;

namespace Engine {

    class Input;

    struct WindowDesc {
        uint32_t width = 1280;
        uint32_t height = 720;
        std::string title = "AutoChess Engine";
    };

    class Window {
    public:
        Window() = default;
        ~Window();

        void Create(const WindowDesc& desc);
        void Destroy();

        void PollEvents();

        bool ShouldClose() const;

        GLFWwindow* GetNativeHandle() const;

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        uint32_t GetFramebufferWidth() const;
        uint32_t GetFramebufferHeight() const;

        bool WasResized() const;
        void ResetResizeFlag();

        Input& GetInput();

    private:
        void UpdateFramebufferSize();

    private:
        GLFWwindow* m_Window = nullptr;

        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

        uint32_t m_FramebufferWidth = 0;
        uint32_t m_FramebufferHeight = 0;

        bool m_FramebufferResized = false;

        Input* m_Input = nullptr;
    };

}