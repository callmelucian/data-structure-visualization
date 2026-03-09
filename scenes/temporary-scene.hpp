#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "scene-manager.hpp"

#include "../include/utility.hpp"

#include "../components/button.hpp"
#include "../components/action-bar.hpp"
#include "../components/global-setting.hpp"
#include "../components/slider.hpp"

class TestButtonScene : public Scene {
private:
    ActionBar actionBar;
    Button button1, button2;
    Slider slider;

public:
    // constructor
    TestButtonScene (const sf::RenderWindow &window) :
    Scene(window), button1(250, 50), button2(250, 50), slider(300, 30) {
        button1.setString("TOGGLE");
        button1.setCallback([&]() {
            button2.toggleState();
            std::cerr << "Button 1 pressed, toggle button 2's state" << std::endl;
        });
        button1.setPosition({700, 400});

        button2.setString("PRESS");
        button2.setCallback([&]() {
            std::cerr << "Button 2 pressed, happi happi" << std::endl;
        });
        button2.setPosition({1100, 400});

        actionBar.setSubtitle("Linked-list");
        actionBar.setPosition({Setting::actionBarWidth / 2.f, Setting::actionBarHeight / 2.f});

        slider.setChangeValueFunction([&] (float value) {
            std::cerr << "Value is set to " << value << std::endl;
        });
        slider.setDisplayValueFunction(floatToPercentage);
        slider.setPosition({900, 500});
    }

    // (override) handle events
    void handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) override {
        button1.handleMouseEvents(window, event);
        button2.handleMouseEvents(window, event);
        actionBar.handleMouseEvents(window, event);
        slider.handleMouseEvents(window, event);
    }

    // (override) time propagation
    void timePropagation (float delta) override {}

    // (override) draw scene onto the screen
    void draw (sf::RenderWindow &window) override {
        window.draw(button1);
        window.draw(button2);
        window.draw(actionBar);
        window.draw(slider);
    }
};