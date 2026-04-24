#include <SFML/Graphics.hpp>
#include <iostream>
#ifdef _WIN32
    #include <windows.h>
#endif

// include scenes
#include "../include/scenes/scene-manager.hpp"
// #include "../include/scenes/avl-tree-scene.hpp"
// #include "../include/scenes/dijkstra-scene.hpp"
// #include "../include/scenes/prim-scene.hpp"
// #include "../include/scenes/rb-tree-scene.hpp"
// #include "../include/scenes/hash-map-scene.hpp"
// #include "../include/scenes/linked-list-scene.hpp"
// #include "../include/scenes/start-scene.hpp"
#include "../include/scenes/test-scene.hpp"

#include "../assets/theme.hpp"

int main() {
    #ifdef _WIN32
        SetProcessDPIAware();
    #endif

    // load assets
    Theme::loadIcons();

    // anti aliasing
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    // initialize window
    auto window = sf::RenderWindow(
        sf::VideoMode({(int)Setting::screenWidth, (int)Setting::screenHeight}),
        "Data Structures Visualizer",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );
    window.setPosition({50, 50});
    window.setFramerateLimit(60);

    // initialize scene manager
    SceneManager manager;
    manager.addNewScene(
        std::make_unique<StartScene>(manager)
    );
    manager.addNewScene(
        std::make_unique<AVLTreeScene>(manager)
    );
    // manager.addNewScene(
    //     std::make_unique<DijkstraScene>(window, manager)
    // );
    // manager.addNewScene(
    //     std::make_unique<HashMapScene>(window, manager)
    // );
    // manager.addNewScene(
    //     std::make_unique<LinkedListScene>(window, manager)
    // );
    // manager.addNewScene(
    //     std::make_unique<PrimScene>(window, manager)
    // );
    // manager.addNewScene(
    //     std::make_unique<RBTreeScene>(window, manager)
    // );
    // manager.addNewScene(
    //     std::make_unique<SettingScene>(window, manager)
    // );
    manager.runMainLoop(window);

    return 0;
}