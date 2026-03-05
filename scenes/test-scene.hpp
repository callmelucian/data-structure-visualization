#pragma once
#include <SFML/Graphics.hpp>
#include "../assets/theme.hpp"
#include "../components/nodes.hpp"
#include "../components/edges.hpp"
#include "../components/button.hpp"
#include "../components/action-bar.hpp"
#include "../components/slider.hpp"
#include "../components/tree.hpp"
#include "scene-manager.hpp"
#include <vector>

class TestScene : public Scene {
private:
    ActionBar actionBar;
    Slider slider;

public:
    TestScene (const sf::RenderWindow &window) :
        Scene(Theme::getBackground(), window), slider(200.f, 40.f, "Volume"),
        actionBar(boundR, actionBarHeight, Theme::getBackground(), sf::Color::Black, "Linked-list") {
            actionBar.setPosition({boundR / 2.f, actionBarHeight / 2.f});

            slider.setPosition({900.f, 400.f});
            slider.setChangeValueFunction([&] (float percentage) {
                std::cerr << "Percentage is set to " << percentage << std::endl;
            });
        }

    void handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
        // Handle event: Mouse hover
        slider.handleMouseHover(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        // Handle event: Mouse pressed
        if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
                slider.handleMouseClick(mousePos);
            }
        }
        // Handle event: Mouse moved
        if (const auto *mouseMovement = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouseMovement->position);
        }
        // Handle event: Mouse released (left-click)
        if (const auto *mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseReleased->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseReleased->position);
            }
        }
    }

    void timePropagation (float deltaTime) {
    }

    void draw (sf::RenderWindow &window) {
        window.draw(actionBar);
        window.draw(slider);
    }
};