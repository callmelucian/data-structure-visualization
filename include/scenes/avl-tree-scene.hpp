#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

// utilities
#include "scene-manager.hpp"
#include "../core/utility.hpp"
#include "../ds/avl-tree.hpp"
#include "../animation/animation-manager.hpp"
#include "../animation/events.hpp"

// UI components
#include "../components/action-bar.hpp"
#include "../components/button.hpp"
#include "../components/binary-tree.hpp"
#include "../components/input-field.hpp"
#include "../components/slider.hpp"

class AVLTreeScene : public Scene {
private:
    UI::Button insertButton, eraseButton;
    UI::TextInputField insertField, eraseField;
    AnimationManager<UI::BinaryTree, DS::AVLTree> treeUI;

    UI::Button playButton;
    UI::Button prevStepButton, prevOperationButton;
    UI::Button nextStepButton, nextOperationButton;

public:
    AVLTreeScene(const sf::RenderWindow &window);
    
    void handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) override;
    void timePropagation(float delta) override;
    void draw(sf::RenderWindow &window) override;
};