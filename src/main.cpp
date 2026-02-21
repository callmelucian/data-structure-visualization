#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    std::cerr << "Application Starting..." << std::endl;
    auto window = sf::RenderWindow(sf::VideoMode({800, 600}), "Data Structures Visualizer");
    std::cerr << "Window Created!" << std::endl;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        // This is where your R-Trees or Matrix visualizations will go!
        window.display();
    }
}