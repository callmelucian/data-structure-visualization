#include <SFML/Graphics.hpp>
#include <iostream>

// include scenes
#include "../include/scenes/scene-manager.hpp"
#include "../include/scenes/avl-tree-scene.hpp"
#include "../include/scenes/dijkstra-scene.hpp"

int main() {
    // anti aliasing
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    // initialize window & fonts
    auto window = sf::RenderWindow(
        sf::VideoMode({(int)Setting::screenWidth, (int)Setting::screenHeight}),
        "Data Structures Visualizer",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );
    window.setPosition({50, 50});
    window.setFramerateLimit(60);
    SceneManager manager;

    manager.changeScene(std::make_unique<DijkstraScene>(window));
    manager.runMainLoop(window);

    return 0;
}