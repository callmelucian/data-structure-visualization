#include "../../include/scenes/scene-manager.hpp"

const float BUTTON_PADDING = 10.f;
const float BUTTON_WIDTH = 170.f;
const float BUTTON_HEIGHT = 40.f;
const float BUTTON_RADIUS = 10.f;
const float BUTTON_MARGIN = 30.f;
const int BUTTON_FONT_SIZE = 19;

const float LARGE_RADIUS = 70.f;
const float MEDIUM_RADIUS = 60.f;
const float SMALL_RADIUS = 50.f;

float calculateContainerWidth (int buttonCount) {
    return BUTTON_WIDTH * buttonCount + BUTTON_PADDING * (buttonCount - 1) + BUTTON_MARGIN * 2;
}

// Scene Implementation
Scene::Scene (SceneManager &manager, int buttonCount, int fieldCount) :
    backgroundColor(Theme::getBackground()), manager(manager),
    container(calculateContainerWidth(buttonCount), LARGE_RADIUS + BUTTON_HEIGHT + 2 * BUTTON_MARGIN + 5.f, 20.f),
    previousScene(MEDIUM_RADIUS, MEDIUM_RADIUS, MEDIUM_RADIUS / 2.f),
    setting(MEDIUM_RADIUS, MEDIUM_RADIUS, MEDIUM_RADIUS / 2.f),
    playButton(LARGE_RADIUS, LARGE_RADIUS, LARGE_RADIUS / 2.f),
    prevStepButton(SMALL_RADIUS, SMALL_RADIUS, SMALL_RADIUS / 2.f),
    prevOperationButton(SMALL_RADIUS, SMALL_RADIUS, SMALL_RADIUS / 2.f),
    nextStepButton(SMALL_RADIUS, SMALL_RADIUS, SMALL_RADIUS / 2.f),
    nextOperationButton(SMALL_RADIUS, SMALL_RADIUS, SMALL_RADIUS / 2.f),
    buttons(buttonCount, UI::Button(BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_RADIUS)),
    fields(fieldCount, UI::TextInputField(1000, 40)) {
    
    // ESC button
    previousScene.setPosition({40 + MEDIUM_RADIUS / 2.f, 40 + MEDIUM_RADIUS / 2.f});
    previousScene.setIcon(Theme::leftIcon, 22.f);
    previousScene.setCallback([&]() {
        manager.toPreviousScene();
    });
    
    // setting button
    setting.setPosition({Setting::screenWidth - 40 - MEDIUM_RADIUS / 2.f, 40 + MEDIUM_RADIUS / 2.f});
    setting.setIcon(Theme::settingIcon, 25.f);
    setting.setCallback([&]() {
        manager.changeScene(5);
    });
    
    // buttons
    playButton.setIcon(Theme::playIcon);
    prevStepButton.setIcon(Theme::leftLeftIcon);
    prevOperationButton.setIcon(Theme::previousIcon);
    nextStepButton.setIcon(Theme::rightRightIcon);
    nextOperationButton.setIcon(Theme::nextIcon);

    playButton.setPosition({Setting::screenWidth / 2.f, 740.f});
    prevStepButton.setPosition({Setting::screenWidth / 2.f - LARGE_RADIUS / 2.f - BUTTON_PADDING - SMALL_RADIUS / 2.f, 740.f});
    prevOperationButton.setPosition({Setting::screenWidth / 2.f - LARGE_RADIUS / 2.f - 2 * BUTTON_PADDING - 3.f * SMALL_RADIUS / 2.f, 740.f});
    nextStepButton.setPosition({Setting::screenWidth / 2.f + LARGE_RADIUS / 2.f + BUTTON_PADDING + SMALL_RADIUS / 2.f, 740.f});
    nextOperationButton.setPosition({Setting::screenWidth / 2.f + LARGE_RADIUS / 2.f + 2 * BUTTON_PADDING + 3.f * SMALL_RADIUS / 2.f, 740.f});

    changePlayButton = [&] (bool f) {
        playButton.setIcon(f ? Theme::pauseIcon : Theme::playIcon);
    };

    // feature buttons
    sf::Vector2f position = {Setting::screenWidth / 2.f - container.getSize().x / 2.f, 810};
    position.x += BUTTON_WIDTH / 2.f + BUTTON_MARGIN;
    for (int i = 0; i < buttonCount; i++) {
        buttons[i].setPosition(position);
        position.x += BUTTON_WIDTH + BUTTON_PADDING;
    }

    // container
    float containerY = (playButton.getPosition().y - LARGE_RADIUS / 2.f - BUTTON_MARGIN
                        + buttons[0].getPosition().y + BUTTON_HEIGHT / 2.f + BUTTON_MARGIN) / 2.f;
    container.setFillColor(Theme::getLightBackground());
    container.setPosition({Setting::screenWidth / 2.f, containerY});
    container.centerOrigin();

    // fields
    for (UI::TextInputField &field : fields)
        field.setPosition({Setting::screenWidth / 2.f, 85});
}

sf::Color Scene::getBackground() { 
    return backgroundColor;
}

void Scene::setBackground (const sf::Color &color) {
    backgroundColor = color;
}

void Scene::baseHandleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    previousScene.handleMouseEvents(window, event);
    setting.handleMouseEvents(window, event);
    playButton.handleMouseEvents(window, event);
    prevStepButton.handleMouseEvents(window, event);
    prevOperationButton.handleMouseEvents(window, event);
    nextStepButton.handleMouseEvents(window, event);
    nextOperationButton.handleMouseEvents(window, event);
    for (UI::Button &button : buttons)
        button.handleMouseEvents(window, event);
    for (UI::TextInputField &field : fields) {
        field.handleMouseEvents(window, event);
        field.handleTextEvents(window, event);
    }
}

void Scene::baseDraw (sf::RenderWindow &window) {
    window.draw(previousScene);
    window.draw(setting);
    window.draw(container);
    window.draw(playButton);
    window.draw(prevStepButton);
    window.draw(prevOperationButton);
    window.draw(nextStepButton);
    window.draw(nextOperationButton);
    for (UI::Button &button : buttons) window.draw(button);
    for (UI::TextInputField &field : fields) window.draw(field);
}

void Scene::disableFields() {
    for (UI::TextInputField &field : fields) field.disable();
}

void Scene::baseTimePropagation (float deltaTime) {
    for (UI::TextInputField &field : fields) field.timePropagation();
}

// SceneManager Implementation
SceneManager::SceneManager() : sceneID(1), nextScene(0), sceneChanged(false), popScene(false) {}

void SceneManager::changeScene (int sceneID, bool reset) {
    nextScene = sceneID, sceneChanged = true;
    if (reset) resetScene(sceneID);
}

void SceneManager::toPreviousScene (bool reset) {
    if (sceneID.size() < 2) return;
    popScene = true;
    if (reset) resetScene(sceneID[(int)sceneID.size() - 2]);
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
            sceneID.push_back(nextScene), sceneChanged = false;
        if (popScene)
            sceneID.pop_back(), popScene = false;

        // event listener loop
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (scenes[sceneID.back()]) {
                scenes[sceneID.back()]->handleEvent(window, event);
            }
        }

        if (scenes[sceneID.back()]) {
            window.clear(scenes[sceneID.back()]->getBackground());

            // time propagation on current scene
            sf::Time elapsed = clock.restart();
            scenes[sceneID.back()]->timePropagation(elapsed.asSeconds());
            
            // draw everything from the scene onto the screen
            scenes[sceneID.back()]->draw(window);
        }

        window.display();
    }
}