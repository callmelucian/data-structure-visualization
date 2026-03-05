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
    inline sf::Font cmuSerif("assets/font-cmu/cmunbx.ttf");

    // Noto Mono family
    inline sf::Font notoCondensedBold("assets/font-noto-mono/NotoSansMono_ExtraCondensed-ExtraBold.ttf");
    inline sf::Font notoCondensed("assets/font-noto-mono/NotoSansMono_ExtraCondensed-Regular.ttf");
    inline sf::Font notoRegular("assets/font-noto-mono/NotoSansMono-Regular.ttf");

    // IBM Plex Mono family
    inline sf::Font ibmRegular("assets/font-ibm-plex/IBMPlexMono-Regular.ttf");
    inline sf::Font ibmItalic("assets/font-ibm-plex/IBMPlexMono-Italic.ttf");
    inline sf::Font ibmBold("assets/font-ibm-plex/IBMPlexMono-Bold.ttf");
    inline sf::Font ibmBoldItalic("assets/font-ibm-plex/IBMPlexMono-BoldItalic.ttf");

    inline sf::Color getBackground() { return LightModeColors::background; }
    inline sf::Color getPrimary() { return LightModeColors::primary; }
    inline sf::Color getFloatColor() { return LightModeColors::floatColor; }
    inline sf::Color getFixedColor() { return LightModeColors::fixedColor; }
};