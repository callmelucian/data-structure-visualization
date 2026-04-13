#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cmath>
#include <functional>
#include <string>
#include <random>

#include "global-setting.hpp"

// Function Declarations
float magnitude(const sf::Vector2f &v);
float squaredMagnitude (const sf::Vector2f &v);
float distance(const sf::Vector2f &a, const sf::Vector2f &b);
float squaredDistance (const sf::Vector2f &a, const sf::Vector2f &b);
float cube(float a);
int convert (const std::string &s);

/**
 * @brief Return a unit vector pointing from a to b
 */
sf::Vector2f unitVector (const sf::Vector2f &a, const sf::Vector2f &b);

bool isPointOnSegment (sf::Vector2f a, sf::Vector2f b, sf::Vector2f p, float epsilon = 5.f);

// Overloading operator declaration
std::ostream& operator<<(std::ostream &oup, const sf::Vector2f &v);

// Global Variable Declarations (using extern)
extern std::function<std::string(float)> floatToPercentage;
extern std::mt19937 rng;

float randFloat(float L, float R);
int randInt(int L, int R);

template <float DurationSeconds>
class CountDownClock {
public:
    CountDownClock();

    sf::Time getRemainingTime() const;
    bool isFinished() const;
    void restart();

private:
    sf::Clock m_clock;
    sf::Time m_startTime;
};

template class CountDownClock<Setting::animationDelay>;