#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

// utilities
#include "scene-manager.hpp"
#include "../core/utility.hpp"
#include "../ds/red-black-tree.hpp"
#include "../animation/animation-manager.hpp"
#include "../animation/events.hpp"

// UI components
#include "../components/button.hpp"
#include "../components/binary-tree.hpp"
#include "../components/input-field.hpp"
#include "../components/slider.hpp"

class RBTreeScene : public Scene {
private:
    AnimationManager<UI::BinaryTree, DS::RedBlackTree> ui;

public:
    RBTreeScene (SceneManager &manager);
    
    void handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) override;
    void timePropagation(float delta) override;
    void draw(sf::RenderWindow &window) override;
};