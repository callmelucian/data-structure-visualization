#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "scene-manager.hpp"
#include "../components/button.hpp"

class TestButtonScene : public Scene {
private:
    Button b1, b2;

public:
    // constructor
    TestButtonScene (const sf::RenderWindow &window) : Scene(window), b1(300, 70), b2(300, 70) {
        b1.setString("TOGGLE");
        b1.setCallback([&]() {
            b2.toggleState();
            std::cerr << "Button 1 pressed, toggle button 2's state" << std::endl;
        });
        b1.setPosition({700, 400});

        b2.setString("PRESS");
        b2.setCallback([&]() {
            std::cerr << "button 2 pressed, happi happi" << std::endl;
        });
        b2.setPosition({1100, 400});
    }

    // (override) handle events
    void handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) override {
        b1.handleMouseEvents(window, event);
        b2.handleMouseEvents(window, event);
    }

    // (override) time propagation
    void timePropagation (float delta) override {}

    // (override) draw scene onto the screen
    void draw (sf::RenderWindow &window) override {
        window.draw(b1);
        window.draw(b2);
    }
};