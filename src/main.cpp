#include <iostream>
#include <SFML/Graphics.hpp>
#include "../components/nodes.hpp"
#include "../components/edges.hpp"
#include "../components/button.hpp"
#include "../components/action-bar.hpp"
#include "../scenes/scene-manager.hpp"
#include "../scenes/linked-list-scene.hpp"
#include "../scenes/test-scene.hpp"

int main() {
    // anti aliasing
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 100;

    // initialize window & fonts
    auto window = sf::RenderWindow(sf::VideoMode({1800, 800}), "Data Structures Visualizer");
    SceneManager manager;

    manager.changeScene(std::make_unique<TestScene>(window));
    manager.runMainLoop(window);

    return 0;
}