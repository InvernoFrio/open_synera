#pragma once

#include <algorithm>
#include <cstdint>

namespace Engine {

    struct PixelRenderConfig {
        uint32_t internalWidth = 426;
        uint32_t internalHeight = 240;

        bool nearestUpscale = true;
        bool integerScale = true;
        bool keepAspectRatio = true;
    };

    struct PixelViewport {
        int32_t x = 0;
        int32_t y = 0;

        uint32_t width = 0;
        uint32_t height = 0;

        float scale = 1.0f;

        bool IsValid() const {
            return width > 0 && height > 0;
        }
    };

    inline float GetPixelInternalAspect(
        const PixelRenderConfig& config
    ) {
        if (config.internalHeight == 0) {
            return 1.0f;
        }

        return
            static_cast<float>(config.internalWidth) /
            static_cast<float>(config.internalHeight);
    }

    inline PixelViewport CalculatePixelViewport(
        uint32_t sourceWidth,
        uint32_t sourceHeight,
        uint32_t targetWidth,
        uint32_t targetHeight,
        bool integerScale,
        bool keepAspectRatio
    ) {
        PixelViewport viewport{};

        if (sourceWidth == 0 ||
            sourceHeight == 0 ||
            targetWidth == 0 ||
            targetHeight == 0) {
            return viewport;
        }

        if (!keepAspectRatio) {
            viewport.x = 0;
            viewport.y = 0;
            viewport.width = targetWidth;
            viewport.height = targetHeight;
            viewport.scale =
                static_cast<float>(targetWidth) /
                static_cast<float>(sourceWidth);

            return viewport;
        }

        float scaleX =
            static_cast<float>(targetWidth) /
            static_cast<float>(sourceWidth);

        float scaleY =
            static_cast<float>(targetHeight) /
            static_cast<float>(sourceHeight);

        float scale =
            std::min(scaleX, scaleY);

        if (integerScale) {
            uint32_t integerScaleValue =
                static_cast<uint32_t>(scale);

            /*
                窗口比内部渲染尺寸小时，integerScaleValue 会是 0。
                这时不能继续用整数缩放，否则 viewport 会变成 0。
                所以小窗口时自动退回浮点缩放。
            */
            if (integerScaleValue >= 1) {
                scale =
                    static_cast<float>(integerScaleValue);
            }
        }

        uint32_t outputWidth =
            static_cast<uint32_t>(
                static_cast<float>(sourceWidth) * scale
                );

        uint32_t outputHeight =
            static_cast<uint32_t>(
                static_cast<float>(sourceHeight) * scale
                );

        /*
            防止浮点误差导致 output 比 target 大 1 像素，
            尤其是窗口高度很窄时容易出现 scissor / viewport 异常。
        */
        outputWidth =
            std::min(outputWidth, targetWidth);

        outputHeight =
            std::min(outputHeight, targetHeight);

        viewport.width = outputWidth;
        viewport.height = outputHeight;

        viewport.x =
            static_cast<int32_t>(
                (targetWidth - outputWidth) / 2
                );

        viewport.y =
            static_cast<int32_t>(
                (targetHeight - outputHeight) / 2
                );

        viewport.scale = scale;

        return viewport;
    }

    inline PixelViewport CalculatePixelViewport(
        const PixelRenderConfig& config,
        uint32_t targetWidth,
        uint32_t targetHeight
    ) {
        return CalculatePixelViewport(
            config.internalWidth,
            config.internalHeight,
            targetWidth,
            targetHeight,
            config.integerScale,
            config.keepAspectRatio
        );
    }

}