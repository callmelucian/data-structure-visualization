#pragma once
#include <SFML/Graphics.hpp>

// utilities
#include "scene-manager.hpp"
#include "../include/utility.hpp"

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
    BinaryTree avlTree;

    int counter;

public:
    AVLTreeScene (const sf::RenderWindow &window) :
        Scene(window), inputFieldButton(100, 30), inputField(150, 30), counter(0) {
            inputFieldButton.setPosition({240, 750});
            inputField.setPosition({115, 750});
            actionBar.setPosition({Setting::actionBarWidth / 2.f, Setting::actionBarHeight / 2.f});
            avlTree.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f});

            actionBar.setSubtitle("AVL Tree");
            inputFieldButton.setCallback([&]() {
                std::string msg = inputField.releaseText();
                avlTree.createNode(msg);
                if (counter) {
                    int parent = (counter - 1) / 2;
                    avlTree.addEdge(parent, counter, counter & 1);
                }
                counter++;
                avlTree.calculatePositions(1600, 400);
                avlTree.centerOrigin();
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
        avlTree.timePropagation(delta);
        avlTree.centerOrigin();
    }

    void draw (sf::RenderWindow &window) override {
        window.draw(avlTree);
        window.draw(actionBar);
        window.draw(inputField);
        window.draw(inputFieldButton);
    }
};