#include "../../include/scenes/scene-manager.hpp"

// Scene Implementation
Scene::Scene (const sf::RenderWindow &window, SceneManager &manager, const sf::Color &init) : backgroundColor(init), manager(manager) {
    // sf::View view = window.getView();
    // sf::Vector2f size = view.getSize();
    // sf::Vector2f center = view.getCenter();

    // sf::Vector2f lo = center - size / 2.f, hi = center + size / 2.f;
    // boundL = lo.x; boundR = hi.x;
    // boundT = lo.y; boundB = hi.y;
}

sf::Color Scene::getBackground() { 
    return backgroundColor; 
}

// SceneManager Implementation
SceneManager::SceneManager() : previousScene(0), currentScene(0), nextScene(0), sceneChanged(false) {}

// void SceneManager::changeScene(std::unique_ptr<Scene> newScene) {
//     // currentScene = std::move(newScene);
//     nextScene = std::move(newScene);
//     sceneChanged = true;
// }

void SceneManager::changeScene (int sceneID, bool reset) {
    nextScene = sceneID, sceneChanged = true;
    if (reset) resetScene(sceneID);
}

void SceneManager::toPreviousScene (bool reset) {
    nextScene = previousScene, sceneChanged = true;
    if (reset) resetScene(previousScene);
}

void SceneManager::addNewScene (std::unique_ptr<Scene> newScene) {
    scenes.push_back(std::move(newScene));
}

void SceneManager::resetScene (int sceneID) {
    // temporary nothing
}

void SceneManager::runMainLoop(sf::RenderWindow &window) {
    sf::Clock clock;
    while (window.isOpen()) {
        if (sceneChanged)
            previousScene = currentScene, currentScene = nextScene, sceneChanged = false;

        // event listener loop
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (scenes[currentScene]) {
                scenes[currentScene]->handleEvent(window, event);
            }
        }

        if (scenes[currentScene]) {
            window.clear(scenes[currentScene]->getBackground());

            // time propagation on current scene
            sf::Time elapsed = clock.restart();
            scenes[currentScene]->timePropagation(elapsed.asSeconds());
            
            // draw everything from the scene onto the screen
            scenes[currentScene]->draw(window);
        }

        window.display();
    }
}