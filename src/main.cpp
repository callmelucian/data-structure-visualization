#include <iostream>
#include <SFML/Graphics.hpp>
#include "../components/nodes.hpp"

int main() {
    // anti aliasing
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 10;

    // initialize window & fonts
    auto window = sf::RenderWindow(sf::VideoMode({800, 600}), "Data Structures Visualizer");
    sf::Font font("assets/font/cmunbx.ttf");

    // initialize nodes
    NodeList nodes;
    nodes.pushNode(40.0, "3", font);
    nodes.pushNode(40.0, "6", font);
    nodes[0]->setPosition({300, 300});
    nodes[1]->setPosition({500, 300});

    // system clock
    sf::Clock clock;

    // main loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            /// === WINDOW EVENTS ===
            if (event->is<sf::Event::Closed>()) window.close();

            /// === MOUSE EVENTS ===
            if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) { // mouse pressed
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
                    nodes.handleMouseClick(mousePos);
                }
            }
            if (const auto *mouseMovement = event->getIf<sf::Event::MouseMoved>()) { // mouse moved
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseMovement->position);
                nodes.handleMouseMovement(mousePos);
            }
            if (const auto *mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) { // mouse released
                if (mouseReleased->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mouseReleased->position);
                    nodes.handleMouseRelease(mousePos);
                }
            }
        }
        window.clear(sf::Color::Black);

        sf::Time elapsed = clock.restart();
        nodes.updatePosition(elapsed.asSeconds());
        for (auto &node : nodes) window.draw(*node);

        // FloatingNode node(50.0, "1", font, sf::Color::Red);
        // node.setPosition({400, 300});
        // window.draw(node);

        window.display();
    }
}