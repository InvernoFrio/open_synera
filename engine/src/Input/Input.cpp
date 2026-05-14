#include "Engine/Input/Input.h"

namespace Engine {

    void Input::BeginFrame() {
        for (int i = 0; i < 3; i++) {
            m_MousePressed[i] = false;
            m_MouseReleased[i] = false;
        }
    }

    void Input::SetMousePosition(double x, double y) {
        m_MouseX = x;
        m_MouseY = y;
    }

    void Input::SetMouseButton(int button, bool pressed) {
        if (button < 0 || button >= 3) {
            return;
        }

        bool wasDown = m_MouseDown[button];

        m_MouseDown[button] = pressed;

        if (pressed && !wasDown) {
            m_MousePressed[button] = true;
        }

        if (!pressed && wasDown) {
            m_MouseReleased[button] = true;
        }
    }

    double Input::GetMouseX() const {
        return m_MouseX;
    }

    double Input::GetMouseY() const {
        return m_MouseY;
    }

    bool Input::IsMouseButtonDown(MouseButton button) const {
        return m_MouseDown[static_cast<int>(button)];
    }

    bool Input::IsMouseButtonPressed(MouseButton button) const {
        return m_MousePressed[static_cast<int>(button)];
    }

    bool Input::IsMouseButtonReleased(MouseButton button) const {
        return m_MouseReleased[static_cast<int>(button)];
    }

}