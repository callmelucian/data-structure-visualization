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

    // main loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear(sf::Color::Black);

        FloatingNode node(50.0, "1", font, sf::Color::Red);
        node.setPosition({400, 300});
        window.draw(node);

        window.display();
    }
}