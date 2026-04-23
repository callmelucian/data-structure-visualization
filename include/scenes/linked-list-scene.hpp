#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

// utilities
#include "scene-manager.hpp"
#include "../core/utility.hpp"
#include "../ds/linked-list.hpp"
#include "../animation/animation-manager.hpp"
#include "../animation/events.hpp"

// UI components
#include "../components/action-bar.hpp"
#include "../components/button.hpp"
#include "../components/linked-list.hpp"
#include "../components/input-field.hpp"
#include "../components/slider.hpp"

class LinkedListScene : public Scene {
private:
    UI::Button backButton, settingButton;
    UI::Button insertButton, eraseButton;
    UI::TextInputField insertField, eraseField;
    AnimationManager<UI::LinkedList, DS::LinkedList> treeUI;

    UI::Button playButton;
    UI::Button prevStepButton, prevOperationButton;
    UI::Button nextStepButton, nextOperationButton;

public:
    LinkedListScene(const sf::RenderWindow &window, SceneManager &manager);
    
    void handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) override;
    void timePropagation(float delta) override;
    void draw(sf::RenderWindow &window) override;
};