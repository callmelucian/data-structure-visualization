#include "../../include/scenes/test-scene.hpp"

TestScene::TestScene (SceneManager &manager) : Scene(manager) {}

void TestScene::handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    handleButtons(window, event);

    featureButtons[0].setString("EDIT WEIGHT");
    featureButtons[0].setCharacterSize(BUTTON_FONT_SIZE);

    featureButtons[1].setString("ADD NODE");
    featureButtons[1].setCharacterSize(BUTTON_FONT_SIZE);

    featureButtons[2].setString("DELETE");
    featureButtons[2].setCharacterSize(BUTTON_FONT_SIZE);

    featureButtons[3].setString("RUN FROM");
    featureButtons[3].setCharacterSize(BUTTON_FONT_SIZE);

    featureButtons[4].setString("IMPORT FILE");
    featureButtons[4].setCharacterSize(BUTTON_FONT_SIZE);
}

void TestScene::timePropagation(float delta) {
}

void TestScene::draw(sf::RenderWindow &window) {
    drawButtons(window);
}