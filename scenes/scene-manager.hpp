#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "../assets/theme.hpp"

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

    // constructor
    Scene (const sf::RenderWindow &window, const sf::Color &init = Theme::getBackground()) : backgroundColor(init) {
        sf::View view = window.getView();
        sf::Vector2f size = view.getSize();
        sf::Vector2f center = view.getCenter();

        sf::Vector2f lo = center - size / 2.f, hi = center + size / 2.f;
        boundL = lo.x, boundR = hi.x;
        boundT = lo.y, boundB = hi.y;
    }

    // get background color
    sf::Color getBackground() { return backgroundColor; }

    // virtual destructor
    virtual ~Scene() {}

    // handle event
    virtual void handleEvent (sf::RenderWindow& window, const std::optional<sf::Event> &event) = 0;

    // time propagation
    virtual void timePropagation (float deltaTime) = 0;

    // draw everything of this scene onto the screen (call all of the window.draw(...) functions)
    virtual void draw (sf::RenderWindow &window) = 0;
};

class SceneManager {
private:
    std::unique_ptr<Scene> currentScene;
public:
    // constructor
    SceneManager() : currentScene(nullptr) {}

    // change new scene
    void changeScene (std::unique_ptr<Scene> newScene) {
        currentScene = std::move(newScene);
    }

    // run main loop
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