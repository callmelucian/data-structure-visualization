#include "../../include/scenes/setting-scene.hpp"

SettingScene::SettingScene (SceneManager &manager) :
    Scene(manager), animationSpeed(500, 60, 10), themeColor(500, 60, 10),
    labels(2, UI::Text(Theme::googleSansItalic)), title(Theme::googleSansExBold) {

    labels[0].setString("Animation Speed");
    labels[0].setCharacterSize(22);
    labels[0].setPosition({Setting::screenWidth / 2.f, 300.f});

    animationSpeed.setPosition({Setting::screenWidth / 2.f, 350.f});
    animationSpeed.setChangeValueFunction([&] (float value) {
        Setting::animationCoefficient = std::max(1.f, value * 100.f);
    });
    animationSpeed.setCharacterSize(20);

    labels[1].setString("Theme Color");
    labels[1].setCharacterSize(22);
    labels[1].setPosition({Setting::screenWidth / 2.f, 540.f});

    themeColor.setString(Theme::getThemeName(), 20);
    themeColor.setPosition({Setting::screenWidth / 2.f, 590.f});
    themeColor.setCallback([&]() {
        Theme::nextTheme();
        themeColor.setString(Theme::getThemeName(), 20);
    });

    title.setString("SETTING");
    title.setCharacterSize(40);
    title.setPosition({Setting::screenWidth / 2.f, 90.f});
    title.centerOrigin();

    for (UI::Text &text : labels) {
        text.setFillColor(Theme::getTextPrimary());
        text.centerOrigin();
    }
    title.setFillColor(Theme::getTextPrimary());
}

void SettingScene::handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    previousScene.handleMouseEvents(window, event);
    animationSpeed.handleMouseEvents(window, event);
    themeColor.handleMouseEvents(window, event);
}

void SettingScene::timePropagation(float delta) {}

void SettingScene::draw(sf::RenderWindow &window) {
    window.draw(previousScene);
    window.draw(animationSpeed);
    window.draw(themeColor);
    for (UI::Text &text : labels) window.draw(text);
    window.draw(title);
}