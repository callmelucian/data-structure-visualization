#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cmath>
#include <functional>
#include <string>
#include <random>

// Function Declarations
float magnitude(const sf::Vector2f &v);
float distance(const sf::Vector2f &a, const sf::Vector2f &b);
float cube(float a);

// Overloading operator declaration
std::ostream& operator<<(std::ostream &oup, const sf::Vector2f &v);

// Global Variable Declarations (using extern)
extern std::function<std::string(float)> floatToPercentage;
extern std::mt19937 rng;

float randFloat(float L, float R);