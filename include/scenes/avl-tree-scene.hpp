#pragma once
#include <SFML/Graphics.hpp>

// utilities
#include "scene-manager.hpp"
#include "../core/utility.hpp"
#include "../ds/avl-tree.hpp"

// UI components
#include "../components/action-bar.hpp"
#include "../components/button.hpp"
#include "../components/binary-tree.hpp"
#include "../components/input-field.hpp"
#include "../components/slider.hpp"

int convert (const std::string &s) {
    int ans = 0;
    for (char c : s) {
        if (c < '0' || '9' < c) return -1;
        ans = ans * 10 + c - '0';
    }
    return ans;
}

namespace AVL {

enum EventType {
    CreateNode,
    AddEdge,
    SetRootNode,
    CalculatePositions,
    DeleteNode,
    SwapNode
};

class AVLEvent {
    
};

}; // namespace AVL

class AVLTreeScene : public Scene {
private:
    UI::Button insertButton, eraseButton, highlightButton;
    UI::TextInputField insertField, eraseField, highlightField;
    UI::BinaryTree avlTreeUI;
    DS::AVLTree avlTreeLogic;

    int counter;

public:
    AVLTreeScene (const sf::RenderWindow &window) :
        Scene(window), insertButton(100, 30), eraseButton(100, 30), highlightButton(100, 30), insertField(150, 30), eraseField(150, 30), highlightField(150, 30), counter(0) {
            // intialize buttons
            insertButton.setString("INSERT");
            eraseButton.setString("ERASE");

            // set positions for objects
            insertButton.setPosition({240, 850});
            insertField.setPosition({115, 850});
            eraseButton.setPosition({525, 850});
            eraseField.setPosition({400, 850});
            highlightButton.setPosition({810, 850});
            highlightField.setPosition({685, 850});
            avlTreeUI.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f});

            // set callback functions: AVL Tree Logic object
            avlTreeLogic.setCallbackCreateNode([&] (int value, bool isRoot) {
                avlTreeUI.createNode(std::to_string(value), isRoot);
            });
            avlTreeLogic.setCallbackAddEdge([&] (int parent, int node, bool isLeft) {
                avlTreeUI.addEdge(parent, node, isLeft);
            });
            avlTreeLogic.setCallbackChangeRoot([&] (int newRoot) {
                avlTreeUI.setRootNode(newRoot);
            });
            avlTreeLogic.setCallbackReposition([&]() {
                avlTreeUI.calculatePositions();
            });
            avlTreeLogic.setCallbackDeleteNode([&] (int visualID) {
                // std::cerr << "Delete node with visualID " << visualID << std::endl;
                avlTreeUI.deleteNode(visualID);
            });
            avlTreeLogic.setCallbackSwapValue([&] (int a, int b) {
                avlTreeUI.swapNode(a, b);
            });
            avlTreeLogic.setCallbackApplyAnimation([&]() { avlTreeUI.calculatePositions(); });
            avlTreeLogic.setCallbackHighlightNode([&] (int nodeID) {});

            // set callback functions: input field and button for insertion
            insertField.setCallbackFunction([&] (const std::string &msg) {
                int value = convert(msg);
                if (value == -1) {
                    std::cerr << "Input field contains non-numerical value!" << std::endl;
                    return;
                }
                avlTreeLogic.insert(value);
            });
            insertButton.setCallback([&]() {
                insertField.releaseText();
            });

            // set callback functions: input field and button for deletion
            eraseField.setCallbackFunction([&] (const std::string &msg) {
                int value = convert(msg);
                if (value == -1) {
                    std::cerr << "Input field contains non-numerical value!" << std::endl;
                    return;
                }
                avlTreeLogic.erase(value);
            });
            eraseButton.setCallback([&]() {
                eraseField.releaseText();
            });

            // set callback functions: input field and button for highlighting
            highlightField.setCallbackFunction([&] (const std::string &msg) {
                int value = convert(msg);
                avlTreeUI.setHighlight(value);
            });
            highlightButton.setCallback([&]() {
                highlightField.releaseText();
            });
        }
    
    void handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) override {
        insertField.handleMouseEvents(window, event);
        insertField.handleTextEvents(window, event);
        insertButton.handleMouseEvents(window, event);

        eraseField.handleMouseEvents(window, event);
        eraseField.handleTextEvents(window, event);
        eraseButton.handleMouseEvents(window, event);

        highlightField.handleMouseEvents(window, event);
        highlightField.handleTextEvents(window, event);
        highlightButton.handleMouseEvents(window, event);
    }

    void timePropagation (float delta) override {
        insertField.timePropagation();
        eraseField.timePropagation();
        highlightField.timePropagation();
        avlTreeUI.timePropagation(delta);
    }

    void draw (sf::RenderWindow &window) override {
        window.draw(avlTreeUI);
        window.draw(insertField);
        window.draw(insertButton);
        window.draw(eraseField);
        window.draw(eraseButton);
        window.draw(highlightField);
        window.draw(highlightButton);
    }
};