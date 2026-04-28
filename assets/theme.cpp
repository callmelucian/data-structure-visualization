#include "theme.hpp"

int themeIterator = 0;

const std::vector<std::string> themeName = {
    "Coffee", "Loopy", "Matcha"
};

const std::vector<sf::Color> backgroundColors = {
    sf::Color({214, 209, 203}),
    sf::Color({255, 220, 227}),
    sf::Color({204, 221, 195})
};

const std::vector<sf::Color> lightBackgroundColors = {
    sf::Color({244, 241, 236}),
    sf::Color({255, 239, 239}),
    sf::Color({228, 237, 223})
};

const std::vector<std::vector<sf::Color>> textColors = {
    {sf::Color::Black, sf::Color({180, 180, 180})},
    {sf::Color::Black, sf::Color({223, 95, 97})},
    {sf::Color::Black, sf::Color({128, 134, 95})}
};

const std::vector<std::vector<sf::Color>> accentColors = {
    {sf::Color({162, 153, 125}), sf::Color({207, 198, 181}), sf::Color({127, 106, 44})},
    {sf::Color({245, 154, 149}), sf::Color({249, 207, 213}), sf::Color({184, 52, 77})},
    {sf::Color({167, 194, 74}), sf::Color({200, 225, 172}), sf::Color({93, 109, 35})}
};

const std::vector<std::vector<sf::Color>> buttonColors = {
    {sf::Color({255, 255, 255}), sf::Color({220, 220, 220}), sf::Color({200, 200, 200})},
    {sf::Color({248, 166, 177}), sf::Color({229, 89, 108}), sf::Color({195, 68, 85})},
    {sf::Color({195, 203, 151}), sf::Color({152, 174, 95}), sf::Color({138, 159, 83})}
};

const std::vector<std::vector<sf::Color>> rbColors = {
    {sf::Color({255, 143, 163}), sf::Color({185, 185, 185})},
    {sf::Color({255, 143, 163}), sf::Color({185, 185, 185})},
    {sf::Color({255, 143, 163}), sf::Color({185, 185, 185})}
};

std::vector<sf::Texture> backgroundTextures(3); // Initialized with size matching themeName

const std::vector<bool> haveBackground = {
    false, true, false
};

namespace Theme {
    sf::Font googleSansRegular("assets/font-google-sans-code/GoogleSansCode-Regular.ttf");
    sf::Font googleSansItalic("assets/font-google-sans-code/GoogleSansCode-Italic.ttf");
    sf::Font googleSansBold("assets/font-google-sans-code/GoogleSansCode-Bold.ttf");
    sf::Font googleSansBoldItalic("assets/font-google-sans-code/GoogleSansCode-BoldItalic.ttf");
    sf::Font googleSansExBold("assets/font-google-sans-code/GoogleSansCode-ExtraBold.ttf");
    sf::Font googleSansExBoldItalic("assets/font-google-sans-code/GoogleSansCode-ExtraBoldItalic.ttf");

    sf::Texture leftIcon;
    sf::Texture leftLeftIcon;
    sf::Texture nextIcon;
    sf::Texture pauseIcon;
    sf::Texture playIcon;
    sf::Texture previousIcon;
    sf::Texture rightRightIcon;
    sf::Texture settingIcon;

    std::optional<sf::Sprite> backgroundImage;

    sf::Color lightBackground = lightBackgroundColors[themeIterator];
    sf::Color background = backgroundColors[themeIterator];
    sf::Color textPrimary = textColors[themeIterator][0];
    sf::Color textSecondary = textColors[themeIterator][1];
    sf::Color accentPrimary = accentColors[themeIterator][0];
    sf::Color accentSecondary = accentColors[themeIterator][1];
    sf::Color accentDark = accentColors[themeIterator][2];
    sf::Color button = buttonColors[themeIterator][0];
    sf::Color buttonHovered = buttonColors[themeIterator][1];
    sf::Color buttonPressed = buttonColors[themeIterator][2];
    sf::Color transparent = sf::Color({0, 0, 0, 0});
    sf::Color rbRed = rbColors[themeIterator][0];
    sf::Color rbBlack = rbColors[themeIterator][1];

    bool loadIcons() {
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

        ok &= backgroundTextures[1].loadFromFile("assets/backgrounds/loopy.png");
        backgroundTextures[1].setSmooth(true);
        return ok;
    }

    void nextTheme() {
        themeIterator++;
        if (themeIterator == (int)themeName.size())
            themeIterator = 0;
        updateTheme();
    }

    void updateTheme() {
        if (haveBackground[themeIterator]) {
            backgroundImage.emplace(backgroundTextures[themeIterator]);
            float scaleX = Setting::screenWidth / backgroundImage->getLocalBounds().size.x;
            float scaleY = Setting::screenHeight / backgroundImage->getLocalBounds().size.y;
            float scale = std::max(scaleX, scaleY);
            backgroundImage->setColor(sf::Color({255, 255, 255, 76}));
            backgroundImage->setScale({scale, scale});
        }
        else backgroundImage = std::nullopt;

        lightBackground = lightBackgroundColors[themeIterator];
        background = backgroundColors[themeIterator];
        textPrimary = textColors[themeIterator][0];
        textSecondary = textColors[themeIterator][1];
        accentPrimary = accentColors[themeIterator][0];
        accentSecondary = accentColors[themeIterator][1];
        accentDark = accentColors[themeIterator][2];
        button = buttonColors[themeIterator][0];
        buttonHovered = buttonColors[themeIterator][1];
        buttonPressed = buttonColors[themeIterator][2];
        transparent = sf::Color({0, 0, 0, 0});
        rbRed = rbColors[themeIterator][0];
        rbBlack = rbColors[themeIterator][1];
    }

    std::string getThemeName() { return themeName[themeIterator]; }
    // sf::Color getLightBackground() { return lightBackgroundColors[themeIterator]; }
    // sf::Color getBackground() { return backgroundColors[themeIterator]; }
    // sf::Color getTextPrimary() { return textColors[themeIterator][0]; }
    // sf::Color getTextSecondary() { return textColors[themeIterator][1]; }
    // sf::Color getAccentPrimary() { return accentColors[themeIterator][0]; }
    // sf::Color getAccentSecondary() { return accentColors[themeIterator][1]; }
    // sf::Color getAccentDark() { return accentColors[themeIterator][2]; }
    // sf::Color getButton() { return buttonColors[themeIterator][0]; }
    // sf::Color getHoveredButton() { return buttonColors[themeIterator][1]; }
    // sf::Color getPressedButton() { return buttonColors[themeIterator][2]; }
    // sf::Color getTransparent() { return sf::Color({0, 0, 0, 0}); }
    // sf::Color getRBRed() { return rbColors[themeIterator][0]; }
    // sf::Color getRBBlack() { return rbColors[themeIterator][1]; }
};