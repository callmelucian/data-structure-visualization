#include <SFML/Graphics.hpp>
#include <iostream>

// include scenes
#include "../include/scenes/scene-manager.hpp"
#include "../include/scenes/avl-tree-scene.hpp"

int main() {
    // GraphicTree tree;

    // for (int i = 0; i < 10; i++) tree.createNode(0);
    // tree.addEdge(0, 1);
    // tree.addEdge(0, 2);
    // tree.addEdge(0, 3);
    // tree.addEdge(2, 4);
    // tree.addEdge(2, 5);
    // tree.addEdge(2, 6);
    // tree.addEdge(2, 7);
    // tree.addEdge(3, 8);
    // tree.addEdge(3, 9);

    // tree.calculatePosition(0, 40);
    // tree.printPosition();

    // return 0;

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

    // manager.changeScene(std::make_unique<TestButtonScene>(window));
    manager.changeScene(std::make_unique<AVLTreeScene>(window));
    manager.runMainLoop(window);

    return 0;
}