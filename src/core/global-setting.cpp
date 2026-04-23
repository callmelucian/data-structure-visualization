#include "../../include/core/global-setting.hpp"

namespace Setting {

float animationCoefficient = 50.f;

float animationFactor() {
    return 1.f + animationCoefficient / 50.f;
}

float animationDelay() {
    return 2.01f - animationCoefficient / 50.f;
}

}; // namespace Setting