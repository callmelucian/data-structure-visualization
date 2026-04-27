#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Setting {
    extern const float actionBarWidth;
    extern const float actionBarHeight;

    extern const float screenWidth;
    extern const float screenHeight;
    extern const sf::Vector2f screenCenter;
    extern const float tightFocusX;
    extern const float narrowFocusX;
    extern const float focusY;
    extern const float codeWidth;
    extern const float offCenter;

    float animationFactor(); // 1 + animationCoefficient / 50
    float animationDelay(); // 6.8 - animationCoefficient / 15
    extern float animationCoefficient;

}; // namespace Setting