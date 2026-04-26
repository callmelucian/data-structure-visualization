#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

// utilities
#include "scene-manager.hpp"
#include "../core/utility.hpp"
#include "../ds/hash-map.hpp"
#include "../animation/animation-manager.hpp"
#include "../animation/events.hpp"

// UI components
#include "../components/button.hpp"
#include "../components/hash-map.hpp"
#include "../components/input-field.hpp"
#include "../components/slider.hpp"

class HashMapScene : public Scene {
private:
    AnimationManager<UI::HashMap, DS::HashMap> ui;
    int updateHold;
    
public:
    HashMapScene (SceneManager &manager);
    
    void handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) override;
    void timePropagation(float delta) override;
    void draw(sf::RenderWindow &window) override;
};