#include <iostream>
#include <SFML/Graphics.hpp>
#include "../components/nodes.hpp"
#include "../components/edges.hpp"

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
    nodes.pushNode(40.0, "7", font);
    nodes[0]->setPosition({300, 200});
    nodes[1]->setPosition({500, 200});
    nodes[2]->setPosition({400, 400});

    // intialize edges
    Edges edge(nodes[0], nodes[1]);

    // system clock
    sf::Clock clock;

    // window boundary
    sf::View view = window.getView();
    sf::Vector2f size = view.getSize();
    sf::Vector2f center = view.getCenter();

    sf::Vector2f lo = center - size / 2.f, hi = center + size / 2.f;
    float boundL = lo.x, boundR = hi.x;
    float boundT = lo.y, boundB = hi.y;

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
        nodes.updatePosition(elapsed.asSeconds(), boundL, boundR, boundT, boundB);
        for (auto &node : nodes) window.draw(*node);
        window.draw(edge);

        window.display();
    }
}