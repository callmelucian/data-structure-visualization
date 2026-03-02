#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

/**
 * This is the global class for all scenes
 * whenever we're implementing a new scene,
 * simply inherit from this class Scene and re-implement the functions
 */
class Scene {
private:
    sf::Color backgroundColor;
    
public:
    float boundL, boundR, boundT, boundB;

    Scene (const sf::Color init, const sf::RenderWindow &window) : backgroundColor(init) {
        sf::View view = window.getView();
        sf::Vector2f size = view.getSize();
        sf::Vector2f center = view.getCenter();

        sf::Vector2f lo = center - size / 2.f, hi = center + size / 2.f;
        boundL = lo.x, boundR = hi.x;
        boundT = lo.y, boundB = hi.y;
    }
    sf::Color getBackground() { return backgroundColor; }

    // even if the inherit class doesn't use these functions
    // it must be initialized so that the SceneManager can communicate with this scene
    virtual ~Scene() {}
    virtual void handleEvent (sf::RenderWindow& window, const std::optional<sf::Event> &event) = 0;
    virtual void timePropagation (float deltaTime) = 0;
    virtual void draw (sf::RenderWindow &window) = 0; // implement all the window.draw(...) here
};

class SceneManager {
private:
    std::unique_ptr<Scene> currentScene;
public:
    SceneManager() : currentScene(nullptr) {}

    void changeScene (std::unique_ptr<Scene> newScene) { currentScene = std::move(newScene); }

    void runMainLoop (sf::RenderWindow &window) {
        sf::Clock clock;
        while (window.isOpen()) {
            // event listener loop
            while (const std::optional<sf::Event> event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();
                currentScene->handleEvent(window, event);
            }
            window.clear(currentScene->getBackground());

            // time propagation on current scene
            sf::Time elapsed = clock.restart();
            currentScene->timePropagation(elapsed.asSeconds());
            
            // draw everything from the scene onto the screen
            currentScene->draw(window);
            window.display();
        }
    }
};