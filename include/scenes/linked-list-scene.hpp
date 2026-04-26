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
#include "../components/button.hpp"
#include "../components/linked-list.hpp"
#include "../components/input-field.hpp"
#include "../components/slider.hpp"

class LinkedListScene : public Scene {
private:
    AnimationManager<UI::LinkedList, DS::LinkedList> ui;
    int insertHold, updateHold;

public:
    LinkedListScene (SceneManager &manager);
    
    void handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) override;
    void timePropagation(float delta) override;
    void draw(sf::RenderWindow &window) override;
};