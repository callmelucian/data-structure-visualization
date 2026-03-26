#include "../../include/core/utility.hpp"

float magnitude(const sf::Vector2f &v) { 
    return std::sqrt(v.x * v.x + v.y * v.y); 
}

float distance(const sf::Vector2f &a, const sf::Vector2f &b) { 
    return magnitude(a - b); 
}

float cube(float a) { 
    return a * a * a; 
}

std::ostream& operator<<(std::ostream &oup, const sf::Vector2f &v) {
    return oup << "(" << v.x << ", " << v.y << ")", oup;
}

// Global Variable Definitions
std::function<std::string(float)> floatToPercentage = [](float value) {
    int percentage = static_cast<int>(std::round(value * 100.0f));
    return std::to_string(percentage) + "%";
};

std::mt19937 rng(21);

float randFloat(float L, float R) {
    return std::uniform_real_distribution<float>(L, R)(rng);
}