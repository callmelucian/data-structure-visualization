#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <vector>
#include <functional>

#include "../assets/theme.hpp"
#include "../components/button.hpp"
#include "../components/input-field.hpp"
#include "../core/global-setting.hpp"
#include "../animation/animation-manager.hpp"

extern const float BUTTON_PADDING;
extern const float BUTTON_WIDTH;
extern const float BUTTON_HEIGHT;
extern const float BUTTON_RADIUS;
extern const float BUTTON_MARGIN;
extern const int BUTTON_FONT_SIZE;

extern const float LARGE_RADIUS;
extern const float MEDIUM_RADIUS;
extern const float SMALL_RADIUS;

float calculateContainerWidth (int buttonCount);

class SceneManager;

class Scene {
private:
    sf::Color backgroundColor;
    UI::RoundedRectangle container;
    SceneManager &manager;

protected:
    UI::Button previousScene, setting;
    UI::Button playButton;
    UI::Button prevStepButton, prevOperationButton;
    UI::Button nextStepButton, nextOperationButton;
    std::vector<UI::Button> buttons;
    std::vector<UI::TextInputField> fields;

    std::function<void(bool)> changePlayButton;
public:
    // constructor
    Scene (SceneManager &manager, int buttonCount = 5, int fieldCount = 2);

    // get background color
    sf::Color getBackground();
    void setBackground (const sf::Color &color);
    void setBackgroundImage (const sf::Texture& texture);

    void baseHandleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event);
    void baseDraw (sf::RenderWindow &window);
    void baseTimePropagation (float deltaTime);
    void disableFields();

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
    std::vector<int> sceneID;
    int nextScene;
    bool sceneChanged, popScene;

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