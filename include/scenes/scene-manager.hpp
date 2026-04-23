#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <vector>

#include "../assets/theme.hpp"

/**
 * This is the global class for all scenes
 * whenever we're implementing a new scene,
 * simply inherit from this class Scene and re-implement the functions
 */
class SceneManager;

class Scene {
private:
    sf::Color backgroundColor;
    SceneManager &manager;
    // float boundL, boundR, boundT, boundB;
    
public:
    // constructor
    Scene (const sf::RenderWindow &window, SceneManager &manager, const sf::Color &init = Theme::getBackground());

    // get background color
    sf::Color getBackground();

    // virtual destructor
    virtual ~Scene() {}

    // handle event
    virtual void handleEvent(sf::RenderWindow& window, const std::optional<sf::Event> &event) = 0;

    // time propagation
    virtual void timePropagation(float deltaTime) = 0;

    // draw everything of this scene onto the screen
    virtual void draw(sf::RenderWindow &window) = 0;
};

class SceneManager {
private:
    // std::unique_ptr<Scene> currentScene, nextScene;
    // bool sceneChanged;
    std::vector<std::unique_ptr<Scene>> scenes;
    int previousScene, currentScene, nextScene;
    bool sceneChanged;

public:
    // constructor
    SceneManager();

    // change new scene
    void changeScene (int sceneID, bool reset = false);

    void toPreviousScene (bool reset = false);

    void addNewScene (std::unique_ptr<Scene> newScene);

    void resetScene (int sceneID);

    // run main loop
    void runMainLoop(sf::RenderWindow &window);
};