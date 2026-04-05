#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

// utilities
#include "scene-manager.hpp"
#include "../core/utility.hpp"
#include "../ds/avl-tree.hpp"
#include "../animation/animation-event.hpp"
#include "../animation/binary-tree-animation.hpp"

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

    UI::Button prevStepButton, prevOperationButton;
    UI::Button nextStepButton, nextOperationButton;

public:
    AVLTreeScene(const sf::RenderWindow &window);
    
    void handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) override;
    void timePropagation(float delta) override;
    void draw(sf::RenderWindow &window) override;
};

// #pragma once
// #include <SFML/Graphics.hpp>

// // utilities
// #include "scene-manager.hpp"
// #include "../core/utility.hpp"
// #include "../ds/avl-tree.hpp"
// #include "../animation/animation-event.hpp"
// #include "../animation/binary-tree-animation.hpp"

// // UI components
// #include "../components/action-bar.hpp"
// #include "../components/button.hpp"
// #include "../components/binary-tree.hpp"
// #include "../components/input-field.hpp"
// #include "../components/slider.hpp"

// class AVLTreeScene : public Scene {
// private:
//     UI::Button insertButton, eraseButton;
//     UI::TextInputField insertField, eraseField;
//     AnimationManager<UI::BinaryTree, DS::AVLTree> treeUI;

//     UI::Button prevStepButton, prevOperationButton;
//     UI::Button nextStepButton, nextOperationButton;

// public:
//     AVLTreeScene (const sf::RenderWindow &window) :
//         Scene(window), insertButton(100, 30), eraseButton(100, 30),
//         insertField(150, 30), eraseField(150, 30),
//         treeUI(UI::BinaryTree()),
//         prevStepButton(50, 30), prevOperationButton(50, 30),
//         nextStepButton(50, 30), nextOperationButton(50, 30) {
//             // intialize buttons
//             insertButton.setString("Insert");
//             eraseButton.setString("Erase");

//             prevStepButton.setString("<");
//             prevOperationButton.setString("<<");
//             nextStepButton.setString(">");
//             nextOperationButton.setString(">>");

//             // set positions for objects
//             insertButton.setPosition({240, 850});
//             insertField.setPosition({115, 850});
//             eraseButton.setPosition({525, 850});
//             eraseField.setPosition({400, 850});

//             prevOperationButton.setPosition({700, 850});
//             prevStepButton.setPosition({760, 850});
//             nextStepButton.setPosition({820, 850});
//             nextOperationButton.setPosition({880, 850});

//             // set callback functions: AVL Tree UI
//             treeUI.setCallbackEnableButtons([&] (int f) {
//                 if (f) {
//                     insertButton.enableButton();
//                     eraseButton.enableButton();
//                 }
//                 else {
//                     insertButton.disableButton();
//                     eraseButton.disableButton();
//                 }
//             });

//             // set callback functions: input field and button for insertion
//             insertField.setCallbackFunction([&] (const std::string &msg) {
//                 int value = convert(msg);
//                 if (value == -1) {
//                     std::cerr << "Input field contains non-numerical value!" << std::endl;
//                     return;
//                 }
//                 treeUI.transformLogic([&] (DS::AVLTree &avl) {
//                     return avl.insert(value), true;
//                 });
//             });
//             insertButton.setCallback([&]() {
//                 insertField.releaseText();
//             });

//             // set callback functions: input field and button for deletion
//             eraseField.setCallbackFunction([&] (const std::string &msg) {
//                 int value = convert(msg);
//                 if (value == -1) {
//                     std::cerr << "Input field contains non-numerical value!" << std::endl;
//                     return;
//                 }
//                 treeUI.transformLogic([&] (DS::AVLTree &avl) {
//                     return avl.erase(value);
//                 });
//             });
//             eraseButton.setCallback([&]() {
//                 eraseField.releaseText();
//             });

//             // set callback functions: previous/next buttons
//             prevOperationButton.setCallback([&]() {
//                 treeUI.previousCompleteState();
//             });
//             prevStepButton.setCallback([&]() {
//                 treeUI.previousState();
//             });
//             nextOperationButton.setCallback([&]() {
//                 treeUI.nextCompleteState();
//             });
//             nextStepButton.setCallback([&]() {
//                 treeUI.nextState();
//             });
//         }
    
//     void handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) override {
//         insertField.handleMouseEvents(window, event);
//         insertField.handleTextEvents(window, event);
//         insertButton.handleMouseEvents(window, event);

//         eraseField.handleMouseEvents(window, event);
//         eraseField.handleTextEvents(window, event);
//         eraseButton.handleMouseEvents(window, event);

//         prevOperationButton.handleMouseEvents(window, event);
//         prevStepButton.handleMouseEvents(window, event);
//         nextOperationButton.handleMouseEvents(window, event);
//         nextStepButton.handleMouseEvents(window, event);
//     }

//     void timePropagation (float delta) override {
//         insertField.timePropagation();
//         eraseField.timePropagation();
//         treeUI.timePropagation(delta);
//     }

//     void draw (sf::RenderWindow &window) override {
//         window.draw(treeUI.getCurrentUI());
//         window.draw(insertField);
//         window.draw(insertButton);
//         window.draw(eraseField);
//         window.draw(eraseButton);
//         window.draw(prevOperationButton);
//         window.draw(prevStepButton);
//         window.draw(nextOperationButton);
//         window.draw(nextStepButton);
//     }
// };