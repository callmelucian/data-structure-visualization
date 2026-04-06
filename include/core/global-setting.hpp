#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Setting {
    inline constexpr const char* appName = "CS163 Visualization App";

    inline constexpr float actionBarWidth = 1800.f;
    inline constexpr float actionBarHeight = 120.f;

    inline constexpr float screenWidth = 1800;
    inline constexpr float screenHeight = 900;

    inline constexpr float animationFactor = 2.f;

    inline constexpr sf::Vector2f screenCenter({screenWidth / 2.f, screenHeight / 2.f});
    inline constexpr float focusX = 1600;
    inline constexpr float focusY = 600;

    inline constexpr float animationDelay = 0.5f;
};