#pragma once
#include <SFML/Graphics.hpp>

// utilities
#include "scene-manager.hpp"
#include "../include/utility.hpp"
#include "../include/avl-tree.hpp"

// UI components
#include "../components/action-bar.hpp"
#include "../components/button.hpp"
#include "../components/binary-tree.hpp"
#include "../components/input-field.hpp"

int convert (const std::string &s) {
    int ans = 0;
    for (char c : s) {
        if (c < '0' || '9' < c) return -1;
        ans = ans * 10 + c - '0';
    }
    return ans;
}

class AVLTreeScene : public Scene {
private:
    ActionBar actionBar;
    Button inputFieldButton;
    TextInputField inputField;
    BinaryTree avlTreeUI;
    DS::AVLTree avlTreeLogic;

    int counter;

public:
    AVLTreeScene (const sf::RenderWindow &window) :
        Scene(window), inputFieldButton(100, 30), inputField(150, 30), counter(0) {
            inputFieldButton.setPosition({240, 850});
            inputField.setPosition({115, 850});
            actionBar.setPosition({Setting::actionBarWidth / 2.f, Setting::actionBarHeight / 2.f});
            avlTreeUI.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f});

            actionBar.setSubtitle("AVL Tree");

            // set callback functions
            avlTreeLogic.setCallbackCreateNode([&] (int value, bool isRoot) {
                int visualID = avlTreeUI.createNode(std::to_string(value), isRoot);
                if (isRoot) {
                    avlTreeUI.calculatePositions(1600, 700);
                    avlTreeUI.centerOrigin();
                }
                return visualID;
            });
            avlTreeLogic.setCallbackAddEdge([&] (int parent, int node, bool isLeft) {
                avlTreeUI.addEdge(parent, node, isLeft);
                avlTreeUI.calculatePositions(1600, 700);
                avlTreeUI.centerOrigin();
            });
            inputField.setCallbackFunction([&] (const std::string &msg) {
                int value = convert(msg);
                if (value == -1) {
                    std::cerr << "Input field contains non-numerical value!" << std::endl;
                    return;
                }
                avlTreeLogic.insert(value);
            });
            inputFieldButton.setCallback([&]() {
                inputField.releaseText();
            });
        }
    
    void handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) override {
        inputField.handleMouseEvents(window, event);
        inputField.handleTextEvents(window, event);
        inputFieldButton.handleMouseEvents(window, event);
        actionBar.handleMouseEvents(window, event);
        // avlTree.handleMouseEvents(window, event);
    }

    void timePropagation (float delta) override {
        inputField.timePropagation();
        avlTreeUI.timePropagation(delta);
    }

    void draw (sf::RenderWindow &window) override {
        window.draw(avlTreeUI);
        // window.draw(actionBar);
        window.draw(inputField);
        window.draw(inputFieldButton);
    }
};