#pragma once
#include <SFML/Graphics/Color.hpp>
#include <string>

// constants
// namespace LightModeColors {
//     inline constexpr sf::Color background({240, 240, 240}); // imperfect white
//     inline constexpr sf::Color primary({0, 0, 0}); // perfect black
//     inline constexpr sf::Color secondary({80, 80, 80});

//     inline constexpr sf::Color floatColor = sf::Color({69, 123, 157}); // blue
//     inline constexpr sf::Color fixedColor = sf::Color({230, 57, 70}); // red

//     inline constexpr sf::Color transparentColor = sf::Color({0, 0, 0, 0});

//     inline constexpr sf::Color pressedButton = sf::Color({200, 200, 200});
//     inline constexpr sf::Color hoveredButton = sf::Color({220, 220, 220});
//     inline constexpr sf::Color button = sf::Color({255, 255, 255});
//     // inline constexpr sf::Color idleButton = sf::Color({150, 150, 150});

//     inline constexpr sf::Color edgeHovered = sf::Color({73, 80, 87}); // grey

//     inline constexpr sf::Color accentMain[2] = {
//         sf::Color({158, 42, 43}), // red
//         sf::Color({0, 0, 0}) // green
//     };

//     inline constexpr sf::Color accentSecondary[2] = {
//         sf::Color({84, 11, 14}), // darker red
//         sf::Color({0, 0, 0}), // darker green
//     };

//     inline constexpr sf::Color RBRed = sf::Color({255, 143, 163});
//     inline constexpr sf::Color RBBlack = sf::Color({185, 185, 185});
// };

inline int themeIterator = 0;

inline const std::vector<std::string> themeName = {
    "Default"
};

inline const std::vector<sf::Color> backgroundColors = {
    sf::Color({214, 209, 203})
};

inline const std::vector<sf::Color> lightBackgroundColors = {
    sf::Color({244, 241, 236})
};

inline const std::vector<std::vector<sf::Color>> textColors = {
    {sf::Color::Black, sf::Color({180, 180, 180})}
};

inline const std::vector<std::vector<sf::Color>> accentColors = {
    {sf::Color({162, 153, 125}), sf::Color({207, 198, 181}), sf::Color({127, 106, 44})}
};

inline const std::vector<std::vector<sf::Color>> buttonColors = {
    {sf::Color({255, 255, 255}), sf::Color({220, 220, 220}), sf::Color({200, 200, 200})}
};

inline const std::vector<std::vector<sf::Color>> rbColors = {
    {sf::Color({255, 143, 163}), sf::Color({185, 185, 185})}
};

// return colors
namespace Theme {
    inline sf::Font cmuSerif("assets/font-cmu/cmunbx.ttf");

    // Noto Mono family
    inline sf::Font notoCondensedBold("assets/font-noto-mono/NotoSansMono_ExtraCondensed-ExtraBold.ttf");
    inline sf::Font notoCondensed("assets/font-noto-mono/NotoSansMono_ExtraCondensed-Regular.ttf");
    inline sf::Font notoRegular("assets/font-noto-mono/NotoSansMono-Regular.ttf");

    // Google Sans Code family
    inline sf::Font googleSansRegular("assets/font-google-sans-code/GoogleSansCode-Regular.ttf");
    inline sf::Font googleSansItalic("assets/font-google-sans-code/GoogleSansCode-Italic.ttf");
    inline sf::Font googleSansBold("assets/font-google-sans-code/GoogleSansCode-Bold.ttf");
    inline sf::Font googleSansBoldItalic("assets/font-google-sans-code/GoogleSansCode-BoldItalic.ttf");
    inline sf::Font googleSansExBold("assets/font-google-sans-code/GoogleSansCode-ExtraBold.ttf");
    inline sf::Font googleSansExBoldItalic("assets/font-google-sans-code/GoogleSansCode-ExtraBoldItalic.ttf");

    // IBM Plex Mono family
    inline sf::Font ibmRegular("assets/font-ibm-plex/IBMPlexMono-Regular.ttf");
    inline sf::Font ibmItalic("assets/font-ibm-plex/IBMPlexMono-Italic.ttf");
    inline sf::Font ibmBold("assets/font-ibm-plex/IBMPlexMono-Bold.ttf");
    inline sf::Font ibmBoldItalic("assets/font-ibm-plex/IBMPlexMono-BoldItalic.ttf");

    // Icons
    inline sf::Texture leftIcon;
    inline sf::Texture leftLeftIcon;
    inline sf::Texture nextIcon;
    inline sf::Texture pauseIcon;
    inline sf::Texture playIcon;
    inline sf::Texture previousIcon;
    inline sf::Texture rightRightIcon;
    inline sf::Texture settingIcon;

    inline bool loadIcons() {
        bool ok = true;
        ok &= leftIcon.loadFromFile("assets/icons/left-icon.png");
        leftIcon.setSmooth(true);
        ok &= leftLeftIcon.loadFromFile("assets/icons/left-left-icon.png");
        leftLeftIcon.setSmooth(true);
        ok &= nextIcon.loadFromFile("assets/icons/next-icon.png");
        nextIcon.setSmooth(true);
        ok &= pauseIcon.loadFromFile("assets/icons/pause-icon.png");
        pauseIcon.setSmooth(true);
        ok &= playIcon.loadFromFile("assets/icons/play-icon.png");
        playIcon.setSmooth(true);
        ok &= previousIcon.loadFromFile("assets/icons/previous-icon.png");
        previousIcon.setSmooth(true);
        ok &= rightRightIcon.loadFromFile("assets/icons/right-right-icon.png");
        rightRightIcon.setSmooth(true);
        ok &= settingIcon.loadFromFile("assets/icons/setting-icon.png");
        settingIcon.setSmooth(true);
        return ok;
    }

    inline void nextTheme() {
        themeIterator++;
        if (themeIterator == themeName.size())
            themeIterator = 0;
    }
    
    inline std::string getThemeName() { return themeName[themeIterator]; }

    inline sf::Color getLightBackground() { return lightBackgroundColors[themeIterator]; }
    inline sf::Color getBackground() { return backgroundColors[themeIterator]; }
    inline sf::Color getTextPrimary() { return textColors[themeIterator][0]; }
    inline sf::Color getTextSecondary() { return textColors[themeIterator][1]; }
    inline sf::Color getAccentPrimary() { return accentColors[themeIterator][0]; }
    inline sf::Color getAccentSecondary() { return accentColors[themeIterator][1]; }
    inline sf::Color getAccentDark() { return accentColors[themeIterator][2]; }
    inline sf::Color getButton() { return buttonColors[themeIterator][0]; }
    inline sf::Color getHoveredButton() { return buttonColors[themeIterator][1]; }
    inline sf::Color getPressedButton() { return buttonColors[themeIterator][2]; }
    inline sf::Color getTransparent() { return sf::Color({0, 0, 0, 0}); }
    inline sf::Color getRBRed() { return rbColors[themeIterator][0]; }
    inline sf::Color getRBBlack() { return rbColors[themeIterator][1]; }

    // inline sf::Color getPrimary() { return LightModeColors::primary; }
    // inline sf::Color getSecondary() { return LightModeColors::secondary; }
    
    // inline sf::Color getFloatColor() { return LightModeColors::floatColor; }
    // inline sf::Color getFixedColor() { return LightModeColors::fixedColor; }

    // inline sf::Color getIdleButton() { return LightModeColors::idleButton; }
    // inline sf::Color getEdgeHovered() { return LightModeColors::edgeHovered; }

    // inline sf::Color getAccentMain (int id) { return LightModeColors::accentMain[id]; }
    // inline sf::Color getAccentSecondary (int id) { return LightModeColors::accentSecondary[id]; }

};