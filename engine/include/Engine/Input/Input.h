#pragma once

namespace Engine {

    enum class MouseButton {
        Left = 0,
        Right = 1,
        Middle = 2
    };

    class Input {
    public:
        void BeginFrame();

        void SetMousePosition(double x, double y);
        void SetMouseButton(int button, bool pressed);

        double GetMouseX() const;
        double GetMouseY() const;

        bool IsMouseButtonDown(MouseButton button) const;
        bool IsMouseButtonPressed(MouseButton button) const;
        bool IsMouseButtonReleased(MouseButton button) const;

    private:
        double m_MouseX = 0.0;
        double m_MouseY = 0.0;

        bool m_MouseDown[3] = {};
        bool m_MousePressed[3] = {};
        bool m_MouseReleased[3] = {};
    };

}