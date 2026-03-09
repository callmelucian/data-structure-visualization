#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cmath>
#include <functional>
#include <string>

float magnitude (const sf::Vector2f &v) { return std::sqrt(v.x * v.x + v.y * v.y); }

float distance (const sf::Vector2f &a, const sf::Vector2f &b) { return magnitude(a - b); }

float cube (float a) { return a * a * a; }

std::ostream& operator<< (std::ostream &oup, const sf::Vector2f &v) {
    return oup << "(" << v.x << ", " << v.y << ")", oup;
}

std::function<std::string(float)> floatToPercentage = [] (float value) {
    int percentage = static_cast<int>(std::round(value * 100.0f));
    return std::to_string(percentage) + "%";
};