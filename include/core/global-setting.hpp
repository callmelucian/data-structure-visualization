#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Setting {
    inline constexpr const char* appName = "CS163 Visualization App";

    inline constexpr float actionBarWidth = 1800.f;
    inline constexpr float actionBarHeight = 120.f;

    inline constexpr float screenWidth = 1800;
    inline constexpr float screenHeight = 900;
    inline constexpr sf::Vector2f screenCenter({screenWidth / 2.f, screenHeight / 2.f});
    inline constexpr float focusX = 1550;
    inline constexpr float focusY = 450;
    inline constexpr float codeWidth = 640;
    inline constexpr float offCenter = 1220;

    float animationFactor(); // 1 + animationCoefficient / 50
    float animationDelay(); // 6.8 - animationCoefficient / 15
    extern float animationCoefficient;

}; // namespace Setting