#include "Engine/Platform/Window.h"
#include "Engine/Input/Input.h"
#include "Engine/Core/Log.h"

#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Engine {

    static void CursorPositionCallback(
        GLFWwindow* glfwWindow,
        double x,
        double y
    ) {
        Window* window =
            static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

        if (!window) {
            return;
        }

        window->GetInput().SetMousePosition(x, y);
    }

    static void MouseButtonCallback(
        GLFWwindow* glfwWindow,
        int button,
        int action,
        int
    ) {
        Window* window =
            static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

        if (!window) {
            return;
        }

        if (button >= 0 && button <= 2) {
            if (action == GLFW_PRESS || action == GLFW_RELEASE) {
                bool pressed = action == GLFW_PRESS;
                window->GetInput().SetMouseButton(button, pressed);
            }
        }
    }

    Window::~Window() {
        Destroy();
    }

    void Window::Create(const WindowDesc& desc) {
        m_Width = desc.width;
        m_Height = desc.height;

        m_Input = new Input();

        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_Window = glfwCreateWindow(
            static_cast<int>(desc.width),
            static_cast<int>(desc.height),
            desc.title.c_str(),
            nullptr,
            nullptr
        );

        if (!m_Window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwSetWindowUserPointer(m_Window, this);

        glfwSetCursorPosCallback(
            m_Window,
            CursorPositionCallback
        );

        glfwSetMouseButtonCallback(
            m_Window,
            MouseButtonCallback
        );

        glfwSetFramebufferSizeCallback(
            m_Window,
            [](GLFWwindow* glfwWindow, int width, int height) {
                Window* window =
                    static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

                if (!window) {
                    return;
                }

                window->m_FramebufferWidth =
                    static_cast<uint32_t>(width);

                window->m_FramebufferHeight =
                    static_cast<uint32_t>(height);

                window->m_FramebufferResized = true;
            }
        );

        UpdateFramebufferSize();

        ENGINE_LOG_INFO("Window created");
    }

    void Window::Destroy() {
        if (m_Window) {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }

        glfwTerminate();

        delete m_Input;
        m_Input = nullptr;
    }

    void Window::PollEvents() {
        m_Input->BeginFrame();
        glfwPollEvents();
        UpdateFramebufferSize();
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    GLFWwindow* Window::GetNativeHandle() const {
        return m_Window;
    }

    uint32_t Window::GetWidth() const {
        return m_Width;
    }

    uint32_t Window::GetHeight() const {
        return m_Height;
    }

    uint32_t Window::GetFramebufferWidth() const {
        return m_FramebufferWidth;
    }

    uint32_t Window::GetFramebufferHeight() const {
        return m_FramebufferHeight;
    }

    bool Window::WasResized() const {
        return m_FramebufferResized;
    }

    void Window::ResetResizeFlag() {
        m_FramebufferResized = false;
    }

    Input& Window::GetInput() {
        return *m_Input;
    }

    void Window::UpdateFramebufferSize() {
        int width = 0;
        int height = 0;

        glfwGetFramebufferSize(
            m_Window,
            &width,
            &height
        );

        if (width < 0) {
            width = 0;
        }

        if (height < 0) {
            height = 0;
        }

        m_FramebufferWidth =
            static_cast<uint32_t>(width);

        m_FramebufferHeight =
            static_cast<uint32_t>(height);
    }

}