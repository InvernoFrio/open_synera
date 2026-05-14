#include "Engine/Platform/Window.h"
#include "Engine/Renderer/Vulkan/VulkanRenderer.h"
#include "Engine/Core/Log.h"

#include "AutoChessGame.h"

#include <chrono>
#include <exception>

int main() {
    try {
        Engine::Window window;

        Engine::WindowDesc desc;
        desc.width = 1600;
        desc.height = 900;
        desc.title = "AutoChess Vulkan Engine";

        window.Create(desc);

        Engine::VulkanRenderer renderer;
        renderer.Init(window);

        AutoChessGame game;

        game.Init(
            window.GetFramebufferWidth(),
            window.GetFramebufferHeight()
        );

        auto lastTime =
            std::chrono::high_resolution_clock::now();

        while (!window.ShouldClose()) {
            auto currentTime =
                std::chrono::high_resolution_clock::now();

            float dt =
                std::chrono::duration<float>(
                    currentTime - lastTime
                ).count();

            lastTime = currentTime;

            window.PollEvents();

            uint32_t framebufferWidth =
                window.GetFramebufferWidth();

            uint32_t framebufferHeight =
                window.GetFramebufferHeight();

            game.Update(
                dt,
                window.GetInput(),
                framebufferWidth,
                framebufferHeight
            );

            renderer.DrawFrame(
                window,
                game.GetCamera(),
                game.GetScene()
            );
        }

        renderer.Shutdown();
        window.Destroy();

        return 0;
    }
    catch (const std::exception& e) {
        ENGINE_LOG_ERROR(e.what());
        return -1;
    }
}