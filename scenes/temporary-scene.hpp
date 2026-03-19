#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "scene-manager.hpp"

#include "../include/utility.hpp"

#include "../components/button.hpp"
#include "../components/action-bar.hpp"
#include "../components/global-setting.hpp"
#include "../components/slider.hpp"
#include "../components/input-field.hpp"
#include "../components/nodes.hpp"
#include "../components/tree.hpp"

class TestButtonScene : public Scene {
private:
    ActionBar actionBar;
    Button button1, button2;
    Slider slider;
    TextInputField inputField;
    GraphicTree tree;

public:
    // constructor
    TestButtonScene (const sf::RenderWindow &window) :
    Scene(window), button1(250, 50), button2(250, 50), slider(300, 30), inputField(250, 50) {
        button1.setString("TOGGLE");
        button1.setCallback([&]() {
            button2.toggleState();
            std::cerr << "Button 1 pressed, toggle button 2's state" << std::endl;
        });
        button1.setPosition({700, 400});

        button2.setString("PRESS");
        button2.setCallback([&]() {
            std::cerr << "Release " << inputField.releaseText() << std::endl;
        });
        button2.setPosition({1100, 400});

        actionBar.setSubtitle("Linked-list");
        actionBar.setPosition({Setting::actionBarWidth / 2.f, Setting::actionBarHeight / 2.f});

        slider.setChangeValueFunction([&] (float value) {
            std::cerr << "Value is set to " << value << std::endl;
        });
        slider.setDisplayValueFunction(floatToPercentage);
        slider.setPosition({900, 500});

        inputField.setPosition({900, 300});

        tree.createNode(0);
        tree.createNode(1);
        tree.createNode(2);
        tree.createNode(3);
        tree.createNode(4);
        tree.createNode(5);
        tree.createNode(6);
        tree.createNode(7);
        tree.createNode(8);
        tree.addEdge(0, 1);
        tree.addEdge(0, 2);
        tree.addEdge(1, 3);
        tree.addEdge(1, 4);
        tree.addEdge(2, 5);
        tree.addEdge(2, 6);
        tree.addEdge(2, 7);
        tree.addEdge(1, 8);

        tree.calculatePosition(1800, 800);
        tree.printPosition();

        tree.centerOrigin();
        tree.setPosition({900, 400});
    }

    // (override) handle events
    void handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) override {
        button1.handleMouseEvents(window, event);
        button2.handleMouseEvents(window, event);
        actionBar.handleMouseEvents(window, event);
        slider.handleMouseEvents(window, event);
        inputField.handleMouseEvents(window, event);
        inputField.handleTextEvents(window, event);
    }

    // (override) time propagation
    void timePropagation (float delta) override {
        inputField.timePropagation();
    }

    // (override) draw scene onto the screen
    void draw (sf::RenderWindow &window) override {
        // window.draw(button1);
        // window.draw(button2);
        // window.draw(actionBar);
        // window.draw(slider);
        // window.draw(inputField);
        window.draw(tree);
    }
};