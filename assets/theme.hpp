#pragma once
#include <SFML/Graphics/Color.hpp>

// constants
namespace LightModeColors {
    inline constexpr sf::Color background({240, 240, 240}); // imperfect white
    inline constexpr sf::Color primary({0, 0, 0}); // perfect black
    inline constexpr sf::Color secondary({80, 80, 80});

    inline constexpr sf::Color floatColor = sf::Color({69, 123, 157}); // blue
    inline constexpr sf::Color fixedColor = sf::Color({230, 57, 70}); // red

    inline constexpr sf::Color transparentColor = sf::Color({0, 0, 0, 0});

    inline constexpr sf::Color pressedButton = sf::Color({200, 200, 200});
    inline constexpr sf::Color hoveredButton = sf::Color({220, 220, 220});
    inline constexpr sf::Color button = sf::Color({255, 255, 255});
    inline constexpr sf::Color idleButton = sf::Color({150, 150, 150});

    inline constexpr sf::Color edgeHovered = sf::Color({73, 80, 87}); // grey

    inline constexpr sf::Color accentMain[2] = {
        sf::Color({158, 42, 43}), // red
        sf::Color({0, 0, 0}) // green
    };

    inline constexpr sf::Color accentSecondary[2] = {
        sf::Color({84, 11, 14}), // darker red
        sf::Color({0, 0, 0}), // darker green
    };

    inline constexpr sf::Color RBRed = sf::Color({255, 143, 163});
    inline constexpr sf::Color RBBlack = sf::Color({185, 185, 185});
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
    inline sf::Color getSecondary() { return LightModeColors::secondary; }
    
    inline sf::Color getFloatColor() { return LightModeColors::floatColor; }
    inline sf::Color getFixedColor() { return LightModeColors::fixedColor; }

    inline sf::Color getPressedButton() { return LightModeColors::pressedButton; }
    inline sf::Color getHoveredButton() { return LightModeColors::hoveredButton; }
    inline sf::Color getButton() { return LightModeColors::button; }
    inline sf::Color getIdleButton() { return LightModeColors::idleButton; }
    inline sf::Color getEdgeHovered() { return LightModeColors::edgeHovered; }

    inline sf::Color getAccentMain (int id) { return LightModeColors::accentMain[id]; }
    inline sf::Color getAccentSecondary (int id) { return LightModeColors::accentSecondary[id]; }

    inline sf::Color getTransparent() { return LightModeColors::transparentColor; }
    inline sf::Color getRBRed() { return LightModeColors::RBRed; }
    inline sf::Color getRBBlack() { return LightModeColors::RBBlack; }
};