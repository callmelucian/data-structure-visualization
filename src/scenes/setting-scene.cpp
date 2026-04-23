#include "../../include/scenes/setting-scene.hpp"

SettingScene::SettingScene (const sf::RenderWindow &window, SceneManager &manager) :
    Scene(window, manager), backButton(150, 30), slider(300, 30) {
    
    // intialize back button
    backButton.setString("Back");
    backButton.setPosition({95, 35});
    backButton.setCallback([&]() { manager.toPreviousScene(); });

    slider.setPosition({
        Setting::screenWidth / 2.f,
        Setting::screenHeight / 2.f
    });
    slider.setChangeValueFunction([&] (float value) {
        Setting::animationCoefficient = std::max(1.f, value * 100.f);
    });
    slider.setDisplayValueFunction(floatToPercentage);
}

void SettingScene::handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    backButton.handleMouseEvents(window, event);
    slider.handleMouseEvents(window, event);
}

void SettingScene::timePropagation(float delta) {}

void SettingScene::draw(sf::RenderWindow &window) {
    window.draw(backButton);
    window.draw(slider);
}