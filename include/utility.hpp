#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

float magnitude (const sf::Vector2f &v) { return std::sqrt(v.x * v.x + v.y * v.y); }

float distance (const sf::Vector2f &a, const sf::Vector2f &b) { return magnitude(a - b); }

float cube (float a) { return a * a * a; }