#pragma once
#include <SFML/Graphics/Color.hpp>

// constants
namespace LightModeColors {
    inline constexpr sf::Color background({240, 240, 240}); // imperfect white
    inline constexpr sf::Color primary({0, 0, 0}); // perfect black

    inline constexpr sf::Color floatColor = sf::Color({69, 123, 157}); // blue
    inline constexpr sf::Color fixedColor = sf::Color({230, 57, 70}); // red
};

// return colors
namespace Theme {
    inline sf::Font cmuSerif("assets/font/cmunbx.ttf");

    inline sf::Color getBackground() { return LightModeColors::background; }
    inline sf::Color getPrimary() { return LightModeColors::primary; }
    inline sf::Color getFloatColor() { return LightModeColors::floatColor; }
    inline sf::Color getFixedColor() { return LightModeColors::fixedColor; }
};