#include "../../include/core/global-setting.hpp"

namespace Setting {

const float actionBarWidth = 1800.f;
const float actionBarHeight = 120.f;

const float screenWidth = 1800;
const float screenHeight = 900;
const sf::Vector2f screenCenter({screenWidth / 2.f, screenHeight / 2.f});
const float tightFocusX = 1080;
const float narrowFocusX = 1550;
const float focusY = 450;
const float codeWidth = 640;
const float offCenter = 1220;

float animationCoefficient = 50.f;

float animationFactor() {
    return 1.f + animationCoefficient / 50.f;
}

float animationDelay() {
    return 2.01f - animationCoefficient / 50.f;
}

}; // namespace Setting