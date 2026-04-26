#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

// utilities
#include "scene-manager.hpp"
#include "start-scene.hpp"
#include "../core/utility.hpp"

// UI components
#include "../components/button.hpp"
#include "../components/slider.hpp"

class SettingScene : public Scene {
private:
    UI::Slider animationSpeed;
    UI::Button themeColor;
    std::vector<UI::Text> labels;
    UI::Text title;

public:
    SettingScene (SceneManager &manager);
    
    void handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) override;
    void timePropagation(float delta) override;
    void draw(sf::RenderWindow &window) override;
};