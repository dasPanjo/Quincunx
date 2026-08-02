#pragma once

#include <cstddef>

namespace Penjin {
    enum class MouseButton {
        Left, Right, Middle,

        Count,
    };

    constexpr std::size_t MouseButtonCount = static_cast<std::size_t>(MouseButton::Count);
}
