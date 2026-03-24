#pragma once
#include <SFML/Graphics.hpp>
#include "../assets/theme.hpp"
#include "../components/nodes.hpp"
#include "../components/edges.hpp"
#include "../components/button.hpp"
#include "../components/action-bar.hpp"
#include "scene-manager.hpp"
#include <vector>

class LinkedListScene : public Scene {
private:
    ActionBar actionBar;
    NodeList nodes;
    std::vector<Edges> edges;
    Button button;

public:
    LinkedListScene (const sf::RenderWindow &window) :
        Scene(Theme::getBackground(), window), button(200, 100, "Button", 40, 3),
        actionBar(boundR, actionBarHeight, Theme::getBackground(), sf::Color::Black, "Linked-list") {
            actionBar.setPosition({boundR / 2.f, actionBarHeight / 2.f});

            // nodes initialization
            nodes.pushNode(25.0, "3", Theme::ibmBold, 30, 2.5);
            nodes.pushNode(25.0, "6", Theme::ibmBold, 30, 2.5);
            nodes.pushNode(25.0, "7", Theme::ibmBold, 30, 2.5);
            nodes[0]->setPosition({300, 200});
            nodes[1]->setPosition({500, 200});
            nodes[2]->setPosition({400, 400});
            
            // edge initialization
            edges.emplace_back(nodes[0], nodes[1], 2.0);

            // button initialization
            button.setCallback([&]() {
                std::cerr << "Button clicked" << std::endl;
                return;
            });
            button.setPosition({900, 400});
        }

    void handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
        // Handle event: Mouse hover
        button.handleHover(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        // Handle event: Mouse pressed
        if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
                nodes.handleMouseClick(mousePos);
                button.handleMousePress(mousePos);
            }
        }
        // Handle event: Mouse moved
        if (const auto *mouseMovement = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouseMovement->position);
            nodes.handleMouseMovement(mousePos);
        }
        // Handle event: Mouse released (left-click)
        if (const auto *mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseReleased->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseReleased->position);
                nodes.handleMouseRelease(mousePos);
                button.handleMouseRelease(mousePos);
            }
        }
    }

    void timePropagation (float deltaTime) {
        nodes.updatePosition(deltaTime, boundL, boundR, boundT, boundB);
    }

    void draw (sf::RenderWindow &window) {
        // for (auto &node : nodes) window.draw(*node);
        // for (auto &edge : edges) window.draw(edge);
        window.draw(actionBar);
        window.draw(button);
    }
};