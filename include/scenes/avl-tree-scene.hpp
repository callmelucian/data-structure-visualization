#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

// utilities
#include "scene-manager.hpp"
#include "start-scene.hpp"
#include "setting-scene.hpp"
#include "../core/utility.hpp"
#include "../ds/avl-tree.hpp"
#include "../animation/animation-manager.hpp"
#include "../animation/events.hpp"

// UI components
#include "../components/button.hpp"
#include "../components/binary-tree.hpp"
#include "../components/input-field.hpp"
#include "../components/slider.hpp"

class AVLTreeScene : public Scene {
private:
    AnimationManager<UI::BinaryTree, DS::AVLTree> ui;
    int updateHold;

public:
    AVLTreeScene (SceneManager &manager);
    
    void handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) override;
    void timePropagation(float delta) override;
    void draw(sf::RenderWindow &window) override;
};