#include "../../include/scenes/scene-manager.hpp"

// Scene Implementation
Scene::Scene(const sf::RenderWindow &window, const sf::Color &init) : backgroundColor(init) {
    sf::View view = window.getView();
    sf::Vector2f size = view.getSize();
    sf::Vector2f center = view.getCenter();

    sf::Vector2f lo = center - size / 2.f, hi = center + size / 2.f;
    boundL = lo.x; boundR = hi.x;
    boundT = lo.y; boundB = hi.y;
}

sf::Color Scene::getBackground() { 
    return backgroundColor; 
}

// SceneManager Implementation
SceneManager::SceneManager() : currentScene(nullptr) {}

void SceneManager::changeScene(std::unique_ptr<Scene> newScene) {
    currentScene = std::move(newScene);
}

void SceneManager::runMainLoop(sf::RenderWindow &window) {
    sf::Clock clock;
    while (window.isOpen()) {
        // event listener loop
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (currentScene) {
                currentScene->handleEvent(window, event);
            }
        }

        if (currentScene) {
            window.clear(currentScene->getBackground());

            // time propagation on current scene
            sf::Time elapsed = clock.restart();
            currentScene->timePropagation(elapsed.asSeconds());
            
            // draw everything from the scene onto the screen
            currentScene->draw(window);
        }

        window.display();
    }
}