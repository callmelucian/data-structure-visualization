#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <fstream>
#include <cmath>
#include <functional>
#include <string>
#include <random>
#include <algorithm>
#include <vector>

#include "global-setting.hpp"

extern const int INPUT_NUMBER_LIMIT;

// Function Declarations
float magnitude(const sf::Vector2f &v);
float squaredMagnitude (const sf::Vector2f &v);
float distance(const sf::Vector2f &a, const sf::Vector2f &b);
float squaredDistance (const sf::Vector2f &a, const sf::Vector2f &b);
float cube(float a);
int convert (const std::string &s);

std::vector<int> stringToNumbers (const std::string &s);

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

std::string openFileDialog();

class CountDownClock {
private:
    sf::Clock internalClock;
    sf::Time startTime;
    float duration;

    sf::Time getRemainingTime() const;
public:
    CountDownClock();
    bool isFinished() const;
    void restart();
    void changeDuration (float newDuration);
};
