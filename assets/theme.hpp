#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>

#include "../include/core/global-setting.hpp"

// Global variables
extern int themeIterator;
extern const std::vector<std::string> themeName;
extern const std::vector<sf::Color> backgroundColors;
extern const std::vector<sf::Color> lightBackgroundColors;
extern const std::vector<std::vector<sf::Color>> textColors;
extern const std::vector<std::vector<sf::Color>> accentColors;
extern const std::vector<std::vector<sf::Color>> buttonColors;
extern const std::vector<std::vector<sf::Color>> rbColors;
extern std::vector<sf::Texture> backgroundTextures;
extern const std::vector<bool> haveBackground;

namespace Theme {
    // Fonts
    extern sf::Font cmuSerif;
    extern sf::Font notoCondensedBold;
    extern sf::Font notoCondensed;
    extern sf::Font notoRegular;
    extern sf::Font googleSansRegular;
    extern sf::Font googleSansItalic;
    extern sf::Font googleSansBold;
    extern sf::Font googleSansBoldItalic;
    extern sf::Font googleSansExBold;
    extern sf::Font googleSansExBoldItalic;
    extern sf::Font ibmRegular;
    extern sf::Font ibmItalic;
    extern sf::Font ibmBold;
    extern sf::Font ibmBoldItalic;

    // Icons
    extern sf::Texture leftIcon;
    extern sf::Texture leftLeftIcon;
    extern sf::Texture nextIcon;
    extern sf::Texture pauseIcon;
    extern sf::Texture playIcon;
    extern sf::Texture previousIcon;
    extern sf::Texture rightRightIcon;
    extern sf::Texture settingIcon;

    // Background Sprite
    extern std::optional<sf::Sprite> backgroundImage;

    extern sf::Color lightBackground;
    extern sf::Color background;
    extern sf::Color textPrimary;
    extern sf::Color textSecondary;
    extern sf::Color accentPrimary;
    extern sf::Color accentSecondary;
    extern sf::Color accentDark;
    extern sf::Color button;
    extern sf::Color buttonHovered;
    extern sf::Color buttonPressed;
    extern sf::Color transparent;
    extern sf::Color rbRed;
    extern sf::Color rbBlack;

    // Functions
    bool loadIcons();
    void nextTheme();
    void updateTheme();

    std::string getThemeName();
    // sf::Color getLightBackground();
    // sf::Color getBackground();
    // sf::Color getTextPrimary();
    // sf::Color getTextSecondary();
    // sf::Color getAccentPrimary();
    // sf::Color getAccentSecondary();
    // sf::Color getAccentDark();
    // sf::Color getButton();
    // sf::Color getHoveredButton();
    // sf::Color getPressedButton();
    // sf::Color getTransparent();
    // sf::Color getRBRed();
    // sf::Color getRBBlack();
};