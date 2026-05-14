#pragma once

#include <cstdint>

namespace Engine {

    template <typename T>
    struct AssetHandle {
        uint32_t id = 0;

        bool IsValid() const {
            return id != 0;
        }
    };

}