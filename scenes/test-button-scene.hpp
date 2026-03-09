#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "scene-manager.hpp"

#include "../components/button.hpp"
#include "../components/action-bar.hpp"
#include "../components/global-setting.hpp"

class TestButtonScene : public Scene {
private:
    ActionBar actionBar;
    Button button1, button2;

public:
    // constructor
    TestButtonScene (const sf::RenderWindow &window) : Scene(window), button1(300, 70), button2(300, 70) {
        button1.setString("TOGGLE");
        button1.setCallback([&]() {
            button2.toggleState();
            std::cerr << "Button 1 pressed, toggle button 2's state" << std::endl;
        });
        button1.setPosition({700, 400});

        button2.setString("PRESS");
        button2.setCallback([&]() {
            std::cerr << "button 2 pressed, happi happi" << std::endl;
        });
        button2.setPosition({1100, 400});

        actionBar.setSubtitle("Linked-list");
        actionBar.setPosition({Setting::actionBarWidth / 2.f, Setting::actionBarHeight / 2.f});
    }

    // (override) handle events
    void handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) override {
        button1.handleMouseEvents(window, event);
        button2.handleMouseEvents(window, event);
        actionBar.handleMouseEvents(window, event);
    }

    // (override) time propagation
    void timePropagation (float delta) override {}

    // (override) draw scene onto the screen
    void draw (sf::RenderWindow &window) override {
        window.draw(button1);
        window.draw(button2);
        window.draw(actionBar);
    }
};