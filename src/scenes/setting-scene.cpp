#include "../../include/scenes/setting-scene.hpp"

SettingScene::SettingScene (SceneManager &manager) :
    Scene(manager), animationSpeed(500, 60, 10), themeColor(500, 60, 10),
    labels(2, UI::Text(Theme::googleSansItalic)), title(Theme::googleSansBold) {

    labels[0].setString("Animation Speed");
    labels[0].setCharacterSize(22);
    labels[0].setPosition({Setting::screenWidth / 2.f, 300.f});
    
    animationSpeed.setPosition({Setting::screenWidth / 2.f, 350.f});
    animationSpeed.setChangeValueFunction([&] (float value) {
        Setting::animationCoefficient = std::max(1.f, value * 100.f);
    });
    animationSpeed.setCharacterSize(20);
    animationSpeed.setOutlineThickness(2);

    labels[1].setString("Theme Color");
    labels[1].setCharacterSize(22);
    labels[1].setPosition({Setting::screenWidth / 2.f, 540.f});
    
    themeColor.setString(Theme::getThemeName(), 20);
    themeColor.setPosition({Setting::screenWidth / 2.f, 590.f});
    themeColor.setCallback([&]() {
        Theme::nextTheme();
        themeColor.setString(Theme::getThemeName(), 20);
    });
    themeColor.setOutlineThickness(2);

    title.setString("SETTING");
    title.setCharacterSize(40);
    title.setPosition({Setting::screenWidth / 2.f, 90.f});
    title.centerOrigin();

    for (UI::Text &text : labels) {
        text.setFillColor(Theme::textPrimary);
        text.centerOrigin();
    }
    title.setFillColor(Theme::textPrimary);
}

void SettingScene::handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    previousScene.handleMouseEvents(window, event);
    animationSpeed.handleMouseEvents(window, event);
    themeColor.handleMouseEvents(window, event);
}

void SettingScene::timePropagation(float delta) {}

void SettingScene::draw(sf::RenderWindow &window) {
    previousScene.changeColor();
    window.draw(previousScene);
    animationSpeed.changeColor();
    window.draw(animationSpeed);
    themeColor.changeColor();
    window.draw(themeColor);
    for (UI::Text &text : labels)
        text.setFillColor(Theme::textPrimary), window.draw(text);
    title.setFillColor(Theme::textPrimary);
    window.draw(title);
}